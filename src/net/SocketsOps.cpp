#include"../../include/net/SocketsOps.h"
#include"../../include/net/InetAddr.h"
#include"../../include/log/Logger.h"
#include<strings.h>
#include<iostream>

#include<fcntl.h>

#include<errno.h>
namespace fei
{
namespace socket
{

void setNonBlock(int sockfd)
{
    int flag = fcntl(sockfd, F_GETFL, NULL);
    flag |= O_NONBLOCK;
    int ret = fcntl(sockfd, F_SETFL, flag);
    if(ret < 0) 
    {
        perror("setNonBlock()");
    }
}
int createNonBlock()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd < 0)
    {
        perror("Socket error");
    }
    //setNonBlock(sockfd);
    return sockfd;
}


int bind(int sockfd,  struct sockaddr_in& addr,socklen_t addrlen)
{
    int ret = ::bind(sockfd, (struct sockaddr*)(&addr), addrlen);
    if(ret < 0)
    {
        std::cout<<errno<<std::endl;
        perror("bind");
        LOG_FATAL <<"bind fd"<<sockfd<< "error";
    }
}
int listen(int sockfd, int backlog)
{
    int ret  = ::listen(sockfd, backlog);
    if(ret < 0)
    {
        perror("Listen()");
    }
}
//int accept(int sockfd, struct sockaddr_in& addr, socklen_t* addrlen);

int accept(int sockfd, struct sockaddr_in& addr, socklen_t* addrlen)
{
    int ret = ::accept4(sockfd, reinterpret_cast<struct sockaddr*>(&addr), addrlen, SOCK_NONBLOCK);
    if(ret < 0)
    {
        perror("Accept()");
    }
    return ret;
}
void toHostPort(char* buf, size_t size, struct sockaddr_in& addr)
{
    snprintf(buf, size, "%u", ntohs(addr.sin_port));
}
void toHostIp(char* buf, size_t size, struct sockaddr_in& addr)
{
    snprintf(buf, size, "%s", inet_ntop(AF_INET, &addr.sin_addr, buf, size));

}
void toHostIpPort(char* buf, size_t size, struct sockaddr_in& addr)
{
    char ip[16] = "INVALID";
    inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
    uint16_t port = ntohs(addr.sin_port);
    snprintf(buf, size, "%s:%u", ip, port);

}



struct sockaddr_in getLocalAddr(int sockfd)
{
    struct sockaddr_in localaddr;
    bzero(&localaddr, sizeof localaddr);
    socklen_t addrlen = sizeof(localaddr);
    if (getsockname(sockfd, (sockaddr*)&localaddr, &addrlen) < 0)
    {
         abort();
    }
    return localaddr;
}


}


}