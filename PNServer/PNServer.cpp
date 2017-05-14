#include "PNServer.h"


PNServer::PNServer(int port_, int maxVal):isRunning(false), port(port_), socketStruct(port_),threadPool(), epollManager(maxVal){
    printf("struct have created, waiting for connect \n");
    start();
    printf("waiting for connect \n");
}

void PNServer::start(){
    if(!isRunning){
        isRunning = true;
        int fdTemp = socketStruct.getFD();
        listen(fdTemp, 10);
        //开始监听
        epollManager.addEvent(fdTemp);
        //将监听套接字描述符传入epoll中
    }

}

PNServer::~PNServer(){
    if(isRunning)
        stop();
}

void PNServer::stop(){
    isRunning = false;
}
