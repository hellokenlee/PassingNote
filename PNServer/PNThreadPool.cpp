#include "PNThreadPool.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <type_traits>

//构造函数
PNThreadPool::PNThreadPool(int threadNum, int taskCapacity) :isRunning(true), threadsNum(threadNum),  taskQueue(taskCapacity){
    isRunning = true;
    //创建相应数量的线程
    createThreads();
}

PNThreadPool::~PNThreadPool(){
    assert(taskQueue.Running());
    stop();
}

//创建相应数量的线程
int PNThreadPool::createThreads(){
    threads = (pthread_t*)malloc(sizeof(pthread_t) * threadsNum);

    for(int i = 0 ; i < threadsNum ; ++i){
        pthread_create(&threads[i] , NULL, threadFunc, this);
        //pthread_detach(threads[i]);
    }

    return 0;
}

//停止线程池, 等待每一个线程join,同时释放掉线程,和mutex,condition变量
void PNThreadPool::stop(){
    if(!isRunning){
        return;
    }
    isRunning = false;

    for(int i = 0 ; i < threadsNum ; ++i){
        pthread_join(threads[i],NULL);///第二个参数可以用来获取线程执行函数得到的返回值
    }
    free(threads);
    threads = nullptr;
}

//添加任务仅任务队列
size_t PNThreadPool::addTask(Task&& task){
    taskQueue.push(std::move(task));
    return taskQueue.size();
}

int PNThreadPool::size(){
    return taskQueue.size();
}

//取任务队列中的任务,若队列中没有任务,阻塞在condition中, 同时释放mutex给其他线程使用
PNThreadPool::Task PNThreadPool::take(){
    Task task = nullptr;
    if(!isRunning){
        return task;
    }
    task = taskQueue.popWait();
    return task;
}

//线程执行的函数
void* PNThreadPool::threadFunc(void *arg){
    pthread_t pid = pthread_self();
    PNThreadPool* pool = static_cast<PNThreadPool*>(arg);

    while( pool -> isRunning){
        PNThreadPool::Task task = pool->take();
        if(!task){
            printf("thread %lu will exit\n" , pid);
            break;
        }

        assert(task);
        task(); ///调用函数对象,可以通过函数对象获取返回值
        task = nullptr;
    }
    return 0;
}


void PNThreadPool::waitJoining(){
    for(int i = 0; i < threadsNum; ++i){
        pthread_join(threads[i], nullptr);
    }
}
