#ifndef PN_CLIENT_SERVICE_H
#define PN_CLIENT_SERVICE_H

/*************************************
*
*              客户端服务
*
**************************************/

class PNClientService{
public:
    PNClientService();
    void start();
    void stop();
    void changeStatus(const int status);
    const int getStatus();
    void sendMsg2Server(const string msg);
    void sendMsg2User(const string userName, const string msg);
    void saveMsgLog2File(const string userName);
    void getMsgLog(const string userName);
private:
    string serverIP;
    int serverPort;

    int status;
    string userName;
    //消息队列 <senderName, message> (senderName 可能是 "userNanme"(私聊)/"groupName:userName"(群聊))
    queue<pair<string, string>> msgQueue;
};


#endif
