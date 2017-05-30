#include "PNServer.h"
#include <signal.h>
#include <vector>
#include <stdio.h>

PNServer::PNServer(int port_):isRunning(true), socketStruct(port_),threadPool(), epollManager(){
    printf("struct have created, waiting for connect \n");
}

void PNServer::start(){
    if(isRunning){
        int fdTemp = socketStruct.getFD();
        epollManager.addEvent(fdTemp, 1);   //将监听套接字描述符传入epoll中
        listen(fdTemp, 500);    //开始监听
        printf("my ip  %s and port %d \n", socketStruct.getIPAddress().c_str(), socketStruct.getPort());
        process();
    }
}

void PNServer::process(){
    while(1){
        int waitEventsSize = epollManager.eventPoller();
        for(int i = 0; i < waitEventsSize; ++i){
            int eventFD = epollManager.recvEvent[i].data.fd;
            if(eventFD == socketStruct.getFD() ){//如果为监听的FD
                processNewConnection(eventFD);
            }
            else if(epollManager.recvEvent[i].events == EPOLLIN){
                //threadPool.addTask(std::bind(&PNServer::processMessage, this, eventFD));
                processMessage(eventFD);
            }
        }
    }
}


void PNServer::processNewConnection(const int &listenFD){
    //暂时不考虑客户端的信息
    int connFD = accept(listenFD, NULL, NULL);
    if(connFD < 0){
        perror("connect error");
        return;
    }
    epollManager.addEvent(connFD);
}

void PNServer::processMessage(const int &clientFD){
    char recvMessage[1024];
    int messageSize;
    while((messageSize = read(clientFD, recvMessage, 1024)) > 0){
        //do something

    }
    if(messageSize < 0 &&(errno == EAGAIN || errno == EWOULDBLOCK) ){ //
                printf("close successfully %d \n", clientFD);
                //epollManager.delEvent(clientFD);
                return;
    }
    if(messageSize < 0){
        epollManager.delEvent(clientFD);
        return ;
    }
    epollManager.delEvent(clientFD);
}



PNServer::~PNServer(){
    if(isRunning)
        stop();
}

void PNServer::stop(){
    isRunning = false;
    close(socketStruct.getFD());
}

