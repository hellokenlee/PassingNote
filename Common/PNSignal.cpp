#include "PNSignal.h"

std::map<int, std::function<void()>> PNSignal::handlerMap;

void PNSignal::signalHandler(int sig){
    handlerMap[sig]();
    exit(1);
}

void PNSignal::signal(int sig, const std::function<void()> &handler){
    handlerMap[sig] = handler;
    ::signal(sig, signalHandler);
}

