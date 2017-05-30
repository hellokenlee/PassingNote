#include "PNServer/PNServer.h"
#include "PNClientInterface/PNClientInterface.h"
#include "PNClientService/PNClientService.h"
#include "PNServer/PNEpollManager.h"
#include <iostream>
#include <functional>
#include <signal.h>
#include "Common/PNSignal.h"
// 测试函数
int main(){
    system("pwd");
    PNServer server;
    PNSignal::signal(SIGINT, [&]{server.stop();});
    server.start();
    return 0;
}
