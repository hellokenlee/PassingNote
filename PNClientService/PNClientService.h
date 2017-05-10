#ifndef PN_CLIENT_SERVICE_H
#define PN_CLIENT_SERVICE_H

#include <queue>

using std::queue;
/*************************************
*
*              客户端服务
*
**************************************/

class PNClientService{
public:
    /***
        PNClientService() : 默认构造函数 , 构造时确定端口号 ?

        P.S delete 拷贝构造与赋值
    ***/
    PNClientService();

    /***
        start() : 开启服务器,与服务端建立连接 , status = 1
        stop() : 关闭与服务端的连接 , status = 0
        int getStatus() const  : 返回客户端服务器状态 status
    ***/
    void start();
    void stop();
    const int getStatus(); /// int getStatus() const ; ?
    void changeStatus(const int status);  /// 待商量

    /***
        sendMsg2Server -> 改为sendMsg2Interface ? 将消息队列中的信息返回给客户端interface
        sendMsg2User() -> 发送 msg 给目标userName , 只负责发送到服务端 , 后续转发由客户端处理

        建议添加 :
        void receiveMsgFromInterface() , 从interface中接收相应需要发送给服务端的信息
    ***/
    void sendMsg2Server(const string msg);
    void sendMsg2User(const string userName, const string msg);

    /***
        saveMsgLog2File  保存与userName相应的聊天记录 , 不同userName保存到不同的文件中
        getMsgLog 读取与userName相应的聊天记录
    ***/
    void saveMsgLog2File(const string userName);
    void getMsgLog(const string userName);

private:
    /***
        serverIP : 记录当前客户端的IP地址
        serverPort : connect分配的端口 or 指定端口
    ***/
    string serverIP;
    int serverPort;

    /***
        status : 服务器当前状态  0 : 离线 , 1 : 在线(已连接服务器)
        userName : 用户ID . (不可重复)
    ***/
    int status;
    string userName;

    ///消息队列 <senderName, message> (senderName 可能是 "userNanme"(私聊)/"groupName:userName"(群聊))
    queue<pair<string, string>> msgQueue;
};


#endif
