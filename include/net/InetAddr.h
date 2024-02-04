#ifndef _INETADDR_H_
#define _INETADDR_H_
#include"SocketsOps.h"

#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string>
#include<arpa/inet.h>
namespace fei
{

class InetAddr
{
public:
    InetAddr();
    InetAddr(const struct sockaddr_in& addr);
    

    InetAddr(uint16_t port);
    InetAddr(std::string& ip, int16_t port);
    std::string toPort();
    std::string toIp();
    std::string toIpPort();
    struct sockaddr_in& getAddr();
private:
    struct sockaddr_in addr_;
};

}


#endif