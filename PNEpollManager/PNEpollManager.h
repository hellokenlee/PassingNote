#ifndef PNEPOLLMANAGER_H
#define PNEPOLLMANAGER_H

#include <sys/epoll.h>
class PNEpollManager{
public:
    PNEpollManager(int maxSize= 66666);
    ~PNEpollManager();

    bool addEvent(const int fd, int op = 0);
    bool delEvent(const int fd, int op = 0);
    bool  setEvent(const int fd, int op = 0);

    int getWaitResult() const;
    int readyFD(int ) const;

private:
    PNEpollManager(const PNEpollManager&) = delete;
    PNEpollManager& operator = (const PNEpollManager &) = delete;

    bool setNoBlock(const int fd) ; //如果为ET模式需要将fd设成非阻塞

private:
    int maxEvents;
    int epollFD;
    int currentSize;
    struct epoll_event *recvEvent;

};


#endif // PNEPOLLMANAGER_H
