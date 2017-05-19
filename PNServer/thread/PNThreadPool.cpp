#include "PNThreadPool.h"

PNThreadPool::PNThreadPool(int n ) : stop_(false), tg_(threads_){
    int nthreads = n;
    if(nthreads <= 0){
        nthreads = std::thread::hardware_concurrency(); ///Returns the number of hardware thread contexts.
        nthreads = (nthreads == 0 ? 2 : nthreads);
    }
        std::cout<<"hardware_concurrency " << nthreads <<std::endl;
        ///返回当前计算机最大并发数量.
    for(int i = 0; i != nthreads; ++i){
        threads_.push_back(std::thread([this]{
                                while( !stop_.load(std::memory_order_acquire)){//从内存载入stop变量
                                            task_type task;
                                            {
                                                std::unique_lock<std::mutex> ulk(this->mutex_); ///unique_lock为管理mutex的对象
                                                this ->condition_.wait(ulk, [this]{
                                                                        return stop_.load(std::memory_order_acquire) || !this -> tasks_.empty();
                                                                       }); ///锁于条件变量, 直到stop为true, 或者tasks_队列不为空
                                                if(stop_.load(std::memory_order_acquire))
                                                    return ;
                                                task = std::move(this->tasks_.front());//取第一个任务
                                                this->tasks_.pop();
                                            }
                                            task();
                                       }
                           }));
    }
}

/***
add 操作返回一个future<Type> 类型,使得我们对于每个task get到相应的future返回给主线程,主线程捕获后移动至future的容器中,
从而读取到相应的返回值

***/

