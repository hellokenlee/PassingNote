#ifndef PNThreadPool_H
#define PNThreadPool_H

#include <list>
#include <pthread.h>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>


/************************************

        ThreadPool类, 用于管理多线程

************************************/

template<typename T>
struct SafeQueue: private std::mutex{

    static const int waitInfinite = std::numeric_limits<int>::max();

    SafeQueue(size_t capacity = 0) : capacity(capacity), isRunning(true){}
    //不限制队列容量
    bool push(T&&v);
    T popWait(int waitMs = waitInfinite);
    bool popWait(T *v, int waitMs = waitInfinite);

    size_t size();
    void exit();
    bool Running(){
        return isRunning;
    }

    SafeQueue(SafeQueue &) = delete;
    SafeQueue& operator= (SafeQueue &) = delete;

private:
    std::list<T> items;
    std::condition_variable ready; //条件变量
    size_t capacity;
    std::atomic<bool> isRunning; //是否正在运行
    void waitReady(std::unique_lock<std::mutex>&lk, int waitMs);
};

template<typename T>
void SafeQueue<T>::waitReady(std::unique_lock<std::mutex>& lk, int waitMS){
    if(isRunning == false || !items.empty()){ //当队列不在运行时退出, item任务时退出
        return;
    }
    if(waitMS == waitInfinite){ //无限等待
        ready.wait(lk, [this](){
                        return !isRunning || !items.empty();
                   }); //知道满足条件释放锁
    }
    else if( waitMS > 0){
        auto tp = std::chrono::steady_clock::now() +std::chrono::milliseconds(waitMS); //C++11 获取时间函数
        while(ready.wait_until(lk, tp) != std::cv_status::timeout && items.empty() && isRunning){

        }//持续循环, 直到超时或者来任务
    }
}


template<typename T>
size_t SafeQueue<T>::size(){
    std::lock_guard<std::mutex> lk(*this);
    return items.size();
}


template<typename T>
void SafeQueue<T>::exit(){
    isRunning = false;
    std::lock_guard<std::mutex> lk(*this);
    ready.notify_all();//准备关闭, 通知所有等待事件释放
}

template<typename T>
bool SafeQueue<T>::push(T&& v){ //将变量v push如链表中,  注意传入的是右值引用
    std::lock_guard<std::mutex> lk(*this);
    if(!isRunning || (capacity && items.size() >= capacity)){//超容 (在设置了容量的情况下)
        return false;
    }
    items.push_back(std::move(v));
    ready.notify_one();
    return true;
}

template<typename T>
bool SafeQueue<T>::popWait(T* v, int waitMs){ //引用传参传回
    std::unique_lock<std::mutex> lk(*this); //unique_lock提供更好的控制? 在这里好像没太大区别
    waitReady(lk, waitMs);
    if(items.empty()){
        return false;
    }
    *v = std::move(items.front());
    items.pop_front();
    return true;
}

template<typename T>
T SafeQueue<T>::popWait(int waitMs){
    std::unique_lock<std::mutex>lk(*this);
    waitReady(lk, waitMs);
    if(items.empty())
            return T();//返回一个空
    T r = std::move(items.front());
    items.pop_front();
    return r;
}

/**
template< class R, class... Args >
class function<R(Args...)>
**/
class PNThreadPool{

public:
    typedef std::function<void()> Task;    //function对象, 用于作为线程执行的函数对象

public:
    explicit PNThreadPool(int threadNum = 3, int taskCapacity = 0);    //构造函数
    ~PNThreadPool();  //析构线程池

    size_t addTask(Task&& task);    //添加任务
    size_t addTask(Task &task){
        return addTask(Task(task));
    }
    void stop(); //线程池停止
    int size(); //返回任务队列数量
    Task take(); //取出任务
    void waitJoining();

private:
    int createThreads(); //创建定量线程
    static void* threadFunc(void* threadData); //线程运行的函数,pthread_create 的第三个参数

    //删除拷贝与赋值
private:
    PNThreadPool& operator = (PNThreadPool&) = delete;
    PNThreadPool(const PNThreadPool&) = delete;

private:
    volatile bool isRunning; //判断线程池是否在运行
    int threadsNum; //创建的线程数量
    pthread_t* threads;  //thread创建个数

     SafeQueue<Task> taskQueue; //任务队列
};



#endif // PNThreadPool_H
