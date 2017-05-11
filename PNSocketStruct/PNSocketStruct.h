#ifndef PNSOCKETSTRUCT_H
#define PNSOCKETSTRUCT_H

#include <sys/socket.h>
#include <string>
#include <unistd.h> //close()
class PNSocketStruct{
public:
    PNSocketStruct(int port_ = 0, int fd_ = 0);
    ~PNSocketStruct();

    std::string getIPAddress() const ;
    void setIPAddress(const std::string&);

    int getPort() const;
    void setPort(const int &);

    bool getStatus() const;
    void setStatus(const bool &);

    int getFD() const;
private:
    PNSocketStruct(const PNSocketStruct&) = delete;
    PNSocketStruct& operator=(const PNSocketStruct &) = delete;

    volatile bool isRunning;
    struct sockaddr_in *addr;
    int port;
    int fd;
    std::string IPAddress;

};


#endif // PNSOCKETSTRUCT_H
