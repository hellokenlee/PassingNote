#ifndef PNThreadPool_H
#define PNThreadPool_H

#include <deque>
#include <pthread.h>
#include <functional>



/************************************

        ThreadPool类, 用于管理多线程

************************************/

class PNServer;//声明
/**
template< class R, class... Args >
class function<R(Args...)>
**/
class PNThreadPool{

public:
    typedef std::function<void()> Task;    //function对象, 用于作为线程执行的函数对象

public:
    explicit PNThreadPool(int threadNum = 3);    //构造函数
    ~PNThreadPool();  //析构线程池

    size_t addTask(const Task& task);    //添加任务
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

     std::deque<Task> taskQueue; //任务队列
     pthread_mutex_t mutex; //互斥量,用于并发控制
     pthread_cond_t condition; //条件变量,

};



#endif // PNThreadPool_H
