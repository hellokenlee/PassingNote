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
    PNThreadPool tp;
    for(int i = 0 ; i < 10 ; ++i){
        tp.addTask(std::bind(test, i));
    }
    while(1){
        if(tp.size() == 0){
            tp.stop();
            printf("prepare to stop\n");
            return 0;
        }
    }

    return 0;
}
