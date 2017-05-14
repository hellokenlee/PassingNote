#include "PNEpollManager.h"


PNEpollManager::PNEpollManager(int maxSize): maxEvents(maxSize), currentSize(0){
    epollFD = epoll_create(maxEvents);//创建一个epollFD
    recvEvent = (epoll_event*)malloc(sizeof(epoll_event) * maxEvents);
    //创建对应于相应大小的Event空间
}

PNEpollManager::~PNEpollManager(){
    for(int i = 0 ;i < maxEvents; ++i){
        free(recvEvent[i]);
    }
}

//将传入的FD设为非阻塞
inline bool PNEpollManager::setNoBlock(const int fd){
     if(fcntl(fd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1){
        return false;
    }
    ++currentSize;
    return true;
}

bool PNEpollManager::addEvent(const int fd, int op){ ///单单先考虑读与ET 模式,
    struct epoll_event temp;
    temp.data.fd = fd;
    temp.events = EPOLLIN | EPOLLET;
    if(epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &temp) < 0){
        return false;
    }
    ++currentSize;
    return true;
}

bool PNEpollManager::delEvent(const int fd, int op){
    if(epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, NULL) < 0){
        return false;
    }
    --currentSize;
    return true;
}

inline int PNEpollManager::getWaitResult() const{
    return epoll_wait(epollFD, recvEvent, currentSize, -1);
}


int PNEpollManager::readyFD(int index) const{
    return recvEvent[index].data.fd;
}
