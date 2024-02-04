#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include"InetAddr.h"
#include"Socket.h"
#include"Channel.h"
#include<functional>

namespace fei
{
class EventLoop;
class Acceptor
{
public:
    typedef std::function<void(int , InetAddr&)> newConnectionCallback;
    Acceptor(EventLoop* loop, InetAddr& addr);
    
    
    void handleRead();

    void setnewConnectionCallback(const newConnectionCallback& cb)
    { newConnectionCallback_ = cb; }

    void listen();
private:
    EventLoop* loop_;
    Socket listenSocket_;
    Channel listenChannel_;
    newConnectionCallback newConnectionCallback_;
};


}


#endif