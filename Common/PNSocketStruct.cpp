#include "PNSocketStruct.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <utility>

PNSocketStruct::PNSocketStruct(int port_, int fd_):isRunning(true), addr(nullptr), port(port_), fd(fd_){
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //初始化FD
        printf("socket error\n");
    }
    addr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr)); //初始化socketAddr
    bzero(addr,sizeof(sockaddr));
    addr -> sin_family = AF_INET;
    addr -> sin_addr.s_addr = htonl(INADDR_ANY); //未定义地址
    addr -> sin_port = htons(port); ///这里没解决默认端口的问题

    if(bind(fd, (struct sockaddr*)addr, sizeof(struct sockaddr)) < 0){
        printf("bind error \n");
    }
}


PNSocketStruct::~PNSocketStruct(){
    if(isRunning){
        isRunning = false;
        free(addr);
        close(fd);
    }
}

///返回引用好吗?
int PNSocketStruct::getPort() const {
    if(isRunning){
        return port;
    }
    return -1;
}

std::string PNSocketStruct::getIPAddress() const {
    return IPAddress;
}

bool PNSocketStruct::getStatus() const{
    return isRunning;
}

int PNSocketStruct::getFD()const{
    return fd;
}

