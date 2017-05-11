#include "PNServer.h"


PNServer::PNServer(int port_):isRunning(false), port(port_), socketStruct(port_),threadPool(){
    printf("struct have created, waiting for connect \n");
    start();
    printf("waiting for connect \n");
}

void PNServer::start(){
    if(!isRunning){
        int fdTemp = socketStruct.getFD();
        listen(fdTemp, 10);
        isRunning = true;
        printf("server IPAddress = : %s, Port  = : %d, listenFd  = %d\n", socketStruct.getIPAddress().c_str(), socketStruct.getPort(), socketStruct.getFD());
        int connfd = accept(fdTemp, NULL, NULL);
        printf("connect successfully \n");
        printf("server IPAddress = : %s\n, Port  = : %d, listenFd  = %d\n", socketStruct.getIPAddress().c_str(), socketStruct.getPort(), socketStruct.getFD());

        char buff[1024];
        read(connfd, buff, 1024);
        printf("%s", buff);
        close(connfd);
        printf("close successfully\n");
    }

}

PNServer::~PNServer(){
    if(isRunning)
        stop();
}

void PNServer::stop(){
    isRunning = false;
}
