#ifndef PNSIGNAL_H
#define PNSIGNAL_H

#include <functional>
#include <map>
#include <signal.h>

struct PNSignal{
    static void signal(int , const std::function<void()>& handler);

    static void signalHandler(int);
    static std::map<int, std::function<void()>> handlerMap;
};



#endif // PNSIGNAL_H
