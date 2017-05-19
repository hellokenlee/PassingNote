#ifndef PNThreadPool_H
#define PNThreadPool_H

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <queue>
#include <type_traits>
#include <utility>
#include <vector>
#include <iostream>

class threadsGuard{
public:
    threadsGuard(std::vector<std::thread>& v) : threads_(v){

    }

    ~threadsGuard(){ ///对线程池中的线程进行析构
        for(size_t i = 0; i != threads_.size(); ++i){
            if(threads_[i].joinable()){
                threads_[i].join();
            }
        }
    }

private:
    threadsGuard(threadsGuard&& tg) = delete;///移动构造
    threadsGuard& operator = (threadsGuard &&tg) = delete; ///移动赋值

    threadsGuard(const threadsGuard&) = delete;
    threadsGuard& operator = (const threadsGuard&) = delete;


private:
    std::vector<std::thread>& threads_;
};

class PNThreadPool{
public:
    typedef std::function<void()> task_type; ///任务类型别名

public:

    explicit PNThreadPool(int n = 3);

    ~PNThreadPool(){ //析构
        stop();
        condition_.notify_all();///条件变量通知所有释放
    }

    void stop(){
        stop_.store(true, std::memory_order_release);
    }

    template<class Function, class... Args>
    void add(Function&&, Args&&...);
private:
    std::atomic<bool> stop_; //原子操作 , 是否已经关闭
    std::mutex mutex_; //互斥量
    std::condition_variable condition_;//条件变量

    std::queue<task_type> tasks_;
    std::vector<std::thread> threads_;
    threadsGuard tg_;//这个到底有什么用

};

    template<class Function, class... Args>
    void PNThreadPool::add(Function &&fcn, Args&&...args){ ///为什么要传入右值引用
        typedef typename std::result_of<Function(Args...)>::type return_type;
        /// result_of  -> Obtains the result type of a call to Fn with arguments of the types listed in ArgTypes.
        typedef std::packaged_task<return_type()> task;
        ///A packaged_task wraps a callable element and allows its result to be retrieved asynchronously.
        auto t = std::make_shared<task>(std::bind(std::forward<Function>(fcn), std::forward<Args>(args)...)); ///注意args参数参数与使用是不同的
        auto ret = t->get_future();
        {
            std::lock_guard<std::mutex> lg(this->mutex_);
            if(stop_.load(std::memory_order_acquire))
                throw std::runtime_error("thread pool has stopped");
            tasks_.emplace([t]{
                        (*t)();
                        }); ///捕获只能指针,将函数对象传入队列中

        }
        condition_.notify_one();///通知一个线程
    }

#endif
