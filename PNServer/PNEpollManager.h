#ifndef PNEPOLLMANAGER_H
#define PNEPOLLMANAGER_H

#include <sys/epoll.h>
#include <string.h>
#include <stdio.h>
#include <list>
#include <unordered_set>
#include <mutex>
#include <fcntl.h>

class PNServer;
const int maxEvents = 500; //最大事件数
/***
    需要添加一个管理fd的集合, 以便析构
**/
class PNEpollManager{
    friend PNServer;
public:
    PNEpollManager();
    ~PNEpollManager();

    bool addEvent(const int fd, int op = 0);
    bool delEvent(const int fd, int op = 0);
    bool  setEvent(const int fd, int op = 0);

    int eventPoller(int waitMs = -1);

    inline int readyFD(int ) const;
    inline int getMaxEvents() const;

private:
    PNEpollManager(const PNEpollManager&) = delete;
    PNEpollManager& operator = (const PNEpollManager &) = delete;
    std::unordered_set<int> FDSet;
    inline bool setNoBlock(const int &) ; //如果为ET模式需要将fd设成非阻塞

private:
    int epollFD;
    struct epoll_event recvEvent[maxEvents];
    std::mutex mtx;
};




inline int PNEpollManager::getMaxEvents() const{
    return maxEvents;
}

inline bool PNEpollManager::setNoBlock(const int &fd){
     if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)|O_NONBLOCK) == -1){
        return false;
    }
    return true;
}



#endif // PNEPOLLMANAGER_H
