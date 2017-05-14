#ifndef PN_SERVER_H
#define PN_SERVRE_H

#include <string>
#include <unordered_map>
#include "../PNSocketStruct/PNSocketStruct.h"
#include "../PNThreadPool/PNThreadPool.h"

using std::string;
using std::pair;
using std::unordered_map;
/*************************************
*
*              服务器类
*
**************************************/

class PNServer{
public:
    //构造函数, 默认参数为6666
    PNServer(const int port_ = 6666);
    ~PNServer();
    /***
            start 开启服务,调用listen函数
            stop 关闭服务,close(listenfd)
    ***/
    void start();
    void stop();

    /**
        sendMsg2User 转发来自某客户端的消息,发送给用户userName ,
        函数处理中需要添加上发送方的信息如 将msg 增加至 SourceUser : msg
    **/
    bool sendMsg2User(const std::string userName,const std::string msg);
    /***
        addUser 添加新用户,用于有新的用户登录服务器
    ***/
    void addUser(const std::string userName, pair<std::string,int> address);

    /***
        changeUserAddress  改变用户地址,用户地址包括IP地址 + 端口号, 用于用户再次登录时IP地址与端口号的改变
    ***/
    void changeUserAddress(const std::string userName, pair<std::string,int> address);

    /***
        对用户状态进行操作的成员方法
        setUserStatus  设置用户状态 0 : 离线  1 :　在线
        getUserStatus   返回用户状态
    ***/
    void setUserStatus(const std::string userName, const int status);
    const int getUserStatus(const std::string userName);

private:
    //禁止拷贝和拷贝构造函数操作
    PNServer(const PNServer &other);
    PNServer operator = (const PNServer &other);
private:
    volatile bool isRunning;//服务器运行状态

    PNSocketStruct socketStruct;//服务器网络结构
    PNThreadPool threadPool; //服务器线程池
    PNEpollManager  epollManager;
    unordered_map<std::string, pair<std::string, int>> userAddress;
    ///用户状态<userName, userStatus>
    unordered_map<std::string, int> userStatus;
    ///记录每个用户对应的FD
    unordered_map<std::string,int> userFD;
    ///记录FD对应的userName
    unordered_map<int,std::string> getUserName;
};
/*************************************
    需不需要给服务器添加一个状态 , 考虑添加一个维护状态
    如某些userName由于非法操作或者userName 设置错误导致 成员变量中记录无用的userName
    设置一个函数使服务器进入维护状态       setStatus(int maintainStatus)
    在函数中对成员变量进行维护,维护完后返回监听循环中


*************************************/

#endif
