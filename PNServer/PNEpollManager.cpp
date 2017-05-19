#include "PNEpollManager.h"
#include <stdlib.h>
#include <fcntl.h> //fcntl
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include<stdio.h>

PNEpollManager::PNEpollManager(){
    epollFD = epoll_create(EPOLL_CLOEXEC);//创建一个epollFD
    //创建最大可创建数量
    if(epollFD <0){
        perror("epoll_create error  ");
    }
}

PNEpollManager::~PNEpollManager(){
}

//将传入的FD设为非阻塞
inline bool PNEpollManager::setNoBlock(const int &fd){
     if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)|O_NONBLOCK) == -1){
        return false;
    }
    return true;
}

bool PNEpollManager::addEvent(const int fd, int op){ ///单单先考虑读与ET 模式,
    struct epoll_event temp;
    memset(&temp, 0, sizeof(temp));
    temp.data.fd = fd;
    temp.events = EPOLLIN;
    if(!setNoBlock(fd) || epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &temp) < 0){
        perror("add event Error");
        return false;
    }
    return true;
}

/**
///取消了if语句 ,但不是这里的问题
**/
bool PNEpollManager::delEvent(const int fd, int op){

    if(epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, nullptr) <0){
        //return false;
        perror ("delete error ");
    }
    close(fd);
    return true;
}

