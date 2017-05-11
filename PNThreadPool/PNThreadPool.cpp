#include "PNThreadPool.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

//构造函数
PNThreadPool::PNThreadPool(int threadNum){
    isRunning = true;
    threadsNum = threadNum;
    //创建相应数量的线程
    createThreads();
}

PNThreadPool::~PNThreadPool(){
    stop();
}

//创建相应数量的线程
int PNThreadPool::createThreads(){
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);
    threads = (pthread_t*)malloc(sizeof(pthread_t) * threadsNum);

    for(int i = 0 ; i < threadsNum ; ++i){
        pthread_create(&threads[i] , NULL, threadFunc, this);
    }

    return 0;
}

//停止线程池, 等待每一个线程join,同时释放掉线程,和mutex,condition变量
void PNThreadPool::stop(){
    if(!isRunning){
        return;
    }
    isRunning = false;
    pthread_cond_broadcast(&condition);//让所有条件变量释放

    for(int i = 0 ; i < threadsNum ; ++i){
        pthread_join(threads[i],NULL);///第二个参数可以用来获取线程执行函数得到的返回值
    }
    free(threads);
    threads = nullptr;

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
}

//添加任务仅任务队列
size_t PNThreadPool::addTask(const Task& task){
    pthread_mutex_lock(&mutex);

    taskQueue.push_back(task);
    int size = taskQueue.size();

    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);
    return size;
}

int PNThreadPool::size(){
    pthread_mutex_lock(&mutex);
    int size = taskQueue.size(); //获取任务队列大小
    pthread_mutex_unlock(&mutex);
    return size;
}

//取任务队列中的任务,若队列中没有任务,阻塞在condition中, 同时释放mutex给其他线程使用
PNThreadPool::Task PNThreadPool::take(){
    Task task = nullptr;
    pthread_mutex_lock(&mutex);
    while(taskQueue.empty() && isRunning){
        pthread_cond_wait(&condition, &mutex);
    }

    if(!isRunning){
        pthread_mutex_unlock(&mutex);
        return task;
    }
    assert(!taskQueue.empty());

    task = taskQueue.front();
    taskQueue.pop_front();

    pthread_mutex_unlock(&mutex);
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
        //printf("res : %d,",i);
    }
    return 0;
}
