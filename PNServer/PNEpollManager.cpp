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

int PNEpollManager::eventPoller(int waitMs){ //暂时只处理
    return epoll_wait(epollFD, recvEvent, maxEvents, waitMs);
}

bool PNEpollManager::addEvent(const int fd, int op){ ///单单先考虑读与LT模式

    struct epoll_event temp;
    memset(&temp, 0, sizeof(temp));
    temp.data.fd = fd;
    temp.events = EPOLLIN;

        // 不判断小于0了
    if(setNoBlock(fd) < 0|| epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &temp) < 0){
        close(fd);
        perror("addEvent error");
        return false;
    }
    FDSet.insert(fd);
    return true;
}

/**
///取消了if语句 ,但不是这里的问题
**/
bool PNEpollManager::delEvent(const int fd, int op){
    std::lock_guard<std::mutex> lck(mtx);
    if(FDSet.find(fd) == FDSet.end()){
        return false;
    }
    if(epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, nullptr)< 0){
        perror ("delete fd error");
    }
    if(close(fd) < 0){
        perror("close error");
    }
    FDSet.erase(fd);
    printf("close successful\n");
    return true;
}


