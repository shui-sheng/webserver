#include"../../include/net/Acceptor.h"
#include"../../include/log/Logger.h"
using namespace fei;

Acceptor::Acceptor(EventLoop* loop, InetAddr& addr)
    :loop_(loop),listenSocket_(socket::createNonBlock())
    ,listenChannel_(loop, listenSocket_.Fd())
{
    listenSocket_.setReuseAddr(true);
    listenSocket_.Bind(addr);
    listenChannel_.setReadCallback(std::bind(&Acceptor::handleRead,this));
    //printf("Acceptor Fd=%d\n", listenChannel_.fd());
    LOG_INFO<<"Acceptor_fd = " << listenChannel_.fd();
}

void Acceptor::listen()
{
    listenSocket_.Listen();
    assert(listenSocket_.Fd() == listenChannel_.fd());
    listenChannel_.enableReading();
    //printf("%s over\n", __func__);
}

void Acceptor::handleRead()
{
    InetAddr peerAddr;
    socklen_t addrlen = sizeof(peerAddr);
    int connfd = listenSocket_.Accept(peerAddr, &addrlen);
    if(connfd > 0)
    {
        newConnectionCallback_(connfd, peerAddr);
        LOG_INFO<<"NewConnection Fd=" << connfd << ",peerAddr="<< peerAddr.toIp();
    }

}