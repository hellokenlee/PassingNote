#ifndef PNEPOLLMANAGER_H
#define PNEPOLLMANAGER_H

#include <sys/epoll.h>
#include <string.h>
#include <stdio.h>

const int maxEvents = 2000; //最大事件数
/***
    需要添加一个管理fd的集合, 以便析构
**/
class PNEpollManager{
public:
    PNEpollManager();
    ~PNEpollManager();

    bool addEvent(const int fd, int op = 0);
    bool delEvent(const int fd, int op = 0);
    bool  setEvent(const int fd, int op = 0);

    inline int getWaitResult() const;
    inline int readyFD(int ) const;
    inline int getMaxEvents() const;
    //inline bool isOverload() const;
    //inline int getCurConnected() const;
    struct epoll_event recvEvent[maxEvents];
private:
    PNEpollManager(const PNEpollManager&) = delete;
    PNEpollManager& operator = (const PNEpollManager &) = delete;

    bool setNoBlock(const int &) ; //如果为ET模式需要将fd设成非阻塞

private:
    int epollFD;
};

inline int PNEpollManager::getWaitResult() const{
    int res = epoll_wait(epollFD, (struct epoll_event*)recvEvent, maxEvents, 0);
    return res;
}


inline int PNEpollManager::readyFD(int index) const{
    return recvEvent[index].data.fd;
}


inline int PNEpollManager::getMaxEvents() const{
    return maxEvents;
}

/*inline bool PNEpollManager::isOverload() const{
    return currentSize == maxEvents;
}*/

/*inline int PNEpollManager::getCurConnected() const{
    return currentSize;
}*/


#endif // PNEPOLLMANAGER_H
