#ifndef _SOCKET_H_
#define _SOCKET_H_
#include"SocketsOps.h"

namespace fei
{
class InetAddr;
class Socket
{
public:
    //Socket();
    Socket(int sockfd);
    void Bind(InetAddr& addr);
    void Listen();
    int Accept(InetAddr& addr, socklen_t* addrlen);
    int Fd() { return sockfd_; }
    void setReuseAddr(bool on);

private:

    int sockfd_;
};

}


#endif