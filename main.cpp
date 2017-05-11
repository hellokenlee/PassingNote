#include "PNServer/PNServer.h"
#include "PNClientInterface/PNClientInterface.h"
#include "PNClientService/PNClientService.h"
#include "PNThreadPool/PNThreadPool.h"
#include <iostream>
#include <functional>

int test(int i){
    std::cout<< i << std::endl;
}
// 测试函数
int main(){
    PNServer server;

    return 0;
}
