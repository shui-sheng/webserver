#include"../../include/net/InetAddr.h"
using namespace fei;

InetAddr::InetAddr()
{

}
InetAddr::InetAddr(const struct sockaddr_in& addr)
    :addr_(addr)
{

}
InetAddr::InetAddr(uint16_t port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = htonl(INADDR_ANY);
}

InetAddr::InetAddr(std::string& ip, int16_t port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr.s_addr);
}

std::string InetAddr::toPort() 
{
    char buf[32];
    socket::toHostPort(buf, sizeof(buf), addr_);
    return buf;
}
std::string InetAddr::toIp()
{
    char buf[32];
    socket::toHostIp(buf, sizeof(buf), addr_);
    return buf;
}

std::string InetAddr::toIpPort()
{
    char buf[32];
    socket::toHostIpPort(buf, sizeof(buf), addr_);
    return buf;
}

struct sockaddr_in& InetAddr::getAddr()
{
    return addr_;
}
