#include "PNServer.h"
#include <signal.h>
#include <vector>
#include <stdio.h>
int listenfd;

PNServer::PNServer(int port_, int maxVal):isRunning(false), socketStruct(port_),threadPool(), epollManager(){
    printf("struct have created, waiting for connect \n");
    start();
}

void PNServer::start(){
    if(!isRunning){
        isRunning = true;
        int fdTemp = socketStruct.getFD();
        epollManager.addEvent(fdTemp, 1);   //将监听套接字描述符传入epoll中
        listen(fdTemp, 10000);    //开始监听
        printf("my ip  %s and port %d \n", socketStruct.getIPAddress().c_str(), socketStruct.getPort());
        process();
        listenfd = fdTemp;
    }
}

void PNServer::process(){
    try{
    while(1){
        int waitEventsSize = epollManager.getWaitResult();
        for(int i = 0; i < waitEventsSize; ++i){
            int readyFD = epollManager.readyFD(i);

            //threadPool.addTask(std::bind(&PNServer::func, this, readyFD));
            ///上述为多线程处理
             func(readyFD);
             ///上述为单线程处理

            signal(SIGINT, [](int signo){
                        close(listenfd);
                        exit(1);
                   });
        }
    }
    }catch(std::exception){
        close(socketStruct.getFD());
    }

}

void PNServer::func(int fd){
    if(fd == socketStruct.getFD()){ //监听端口收到消息
        int connFD =  accept(fd, NULL, NULL); ///暂时忽略客户端信息
        if(connFD < 0 &&(errno == EAGAIN || errno == EWOULDBLOCK) ){
            return;
        }
        if( connFD < 0){
            perror("connct error");
            return;
        }
        //printf("A client connected , %d\n", connFD);
        epollManager.addEvent(connFD);

    }
    else{ //为客户端发来的fd
        char buff[1024]; ///改
        int messageSize;
        while((messageSize = read(fd, buff, 1024)) > 0){
            //do something;
        }
        if(messageSize < 0 &&(errno == EAGAIN || errno == EWOULDBLOCK) ){ //
                epollManager.delEvent(fd);
                return;
        }
        if(messageSize < 0){
                epollManager.delEvent(fd); //结束连接, 关闭FD
                //perror("read error or recv RST ");
                return;
        }
        else if(messageSize == 0){
            epollManager.delEvent(fd); //结束连接, 关闭FD
            //printf("close successful\n");
            return ;
        }
    }
}


PNServer::~PNServer(){
    if(isRunning)
        stop();
}

void PNServer::stop(){
    isRunning = false;
}
