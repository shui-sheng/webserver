#include "../../include/net/Server.h"
#include "../../include/net/SocketsOps.h"

#include <functional>
using namespace fei;
using namespace std::placeholders;
Server::Server(EventLoop *loop, InetAddr &listenAddr, std::string name)
    : loop_(loop), acceptor_(new Acceptor(loop, listenAddr)), nextConnId_(1), name_(name), iothreads_(loop_)
{
    //(std::bind(&Server::newConnection, this, _1, _2));
    // LOG_INFO<< "Acceptor fd = " << acceptor_->
    acceptor_->setnewConnectionCallback(std::bind(&Server::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

// be called in acceptor handleRead
void Server::newConnection(int connfd, InetAddr &peerAddr)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "#%d", nextConnId_++);
    std::string newConnname = name_ + buf;
    InetAddr localaddr(socket::getLocalAddr(connfd));

    EventLoop *nextIoLoop = iothreads_.getNextLoop();

    ConnectionPtr conn = new Connection(nextIoLoop, connfd, localaddr, peerAddr, newConnname);


    //
    try
    {
        loop_->assertInLoopThread();
        auto res = connectionMap_.insert({newConnname, conn});
        assert(res.second == true);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        abort();
    }
    

    size_t x = connectionMap_.count(newConnname);
    assert(x == 1);

    conn->setMsgInCallback(newConnCallback_);
    conn->setMsgOutCallback(msgOutCallback_);
    conn->setCloseCallback(std::bind(&Server::removeConnection, this, _1));

    // conn->setCloseCallback(std::bind(&Server::removeConnection, this));

    nextIoLoop->runInLoop(std::bind(&Connection::established, conn));
    assert(connectionMap_.count(newConnname) == 1);

}

void Server::removeConnection(const ConnectionPtr conn)
{
    loop_->queueInLoop(std::bind(&Server::removeConnectionInLoop, this, conn));
}
void Server::removeConnectionInLoop(const ConnectionPtr conn)
{
    loop_->assertInLoopThread();
    std::string name = conn->name();
    size_t x = connectionMap_.count(name);
    LOG_INFO<<"x = " << x;
     assert(x == 1);
    //delete(connectionMap_[name]);
    delete conn;
    connectionMap_.erase(name);
   
}