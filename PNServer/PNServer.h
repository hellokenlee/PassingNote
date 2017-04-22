#ifndef PN_SERVER_H
#define PN_SERVRE_H

/*************************************
*
*              服务器类
*
**************************************/

class PNServer{
public:
    //构造函数
    PNServer(const int port);
    PNServer();
    //启动服务器
    void start();
    //停止服务器
    void stop();
    //根据用户名发送信息到client
    bool sendMsg2User(const string userName,const string msg);
    //增加用户
    void addUser(const string userName, pair<string,int> address);
    //更改用户地址
    void changeUserAddress(const string userName, pair<string,int> address);
    //更改用户状态
    void setUserStatus(const string userName, const int status);
    //查询用户状态
    const int getUserStatus(const string userName);
private:
    //不允许拷贝构造
    PNServer(const PNServer &other);
    PNServer operator = PNServer(const PNServer &other);
private:
    //服务器端口号
    int port;
    //用户地址<userName, <userIP, userPort>>
    unordered_map<string, pair<string, int>> userAddress;
    //用户状态<userName, userStatus>
    unordered_map<string, int> userStatus;
};

#endif
