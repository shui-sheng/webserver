#include"../../include/net/Socket.h"

#include"../../include/net/InetAddr.h"

using namespace fei;

Socket::Socket(int sockfd)
    :sockfd_(sockfd)
{

}
void Socket::Bind(InetAddr& addr)
{
    static_assert(sizeof(InetAddr) == sizeof(struct sockaddr));
    setReuseAddr(true);
    socket::bind(sockfd_, addr.getAddr(), sizeof(addr));
}
void Socket::Listen()
{
    socket::listen(sockfd_);
}
int Socket::Accept(InetAddr& addr, socklen_t* addrlen)
{
    return socket::accept(sockfd_, addr.getAddr(), addrlen);
}
void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
               &optval, sizeof optval);
}