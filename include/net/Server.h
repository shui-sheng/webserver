#ifndef _SERVER_H_
#define _SERVER_H_
#include<functional>
#include<set>
#include"CallBacks.h"
#include"Acceptor.h"
#include"Connection.h"
#include"EventLoopThreadPool.h"
namespace fei
{

class EventLoop;
class Server
{
public:
    typedef Connection* ConnectionPtr;
    typedef std::map<std::string, ConnectionPtr> ConnectionMap_;
    typedef std::function<void(int connfd, InetAddr& peerAddr)> newConnCallback;
    //std::function<void(const ConnectionPtr& conn, Buffer& buffer , const Timestamp& now)>
    typedef std::function<void(const ConnectionPtr& ptr, Buffer& , const Timestamp& now)> msgOutCallback;
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
public:
    
    Server(EventLoop* loop, InetAddr& addr, std::string name);
    //~Server();
    void setThreadInitCallback(const ThreadInitCallback& cb) { threadInitCallback_ = cb; }
    void setNewConnCallback(const newConnCallback& cb) { newConnCallback_ = cb; }
    void setMsgOutCallback(const msgOutCallback& cb) { msgOutCallback_ = cb; }
    void removeConnection(const ConnectionPtr conn);
    //void setCloseCallback(ReadCallback& cb) { closeCallback_ = cb; }
    void start() 
    { 
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_));
        //acceptor_->listen(); 
        iothreads_.start(threadInitCallback_);
    }
    void setThreadNum(int num) { iothreads_.setThreadNum(num); }
private:
    

    EventLoop* loop_;
    Acceptor* acceptor_;
    ConnectionMap_ connectionMap_;
    EventLoopThreadPool iothreads_;
    std::string name_;
    int nextConnId_;

    void newConnection(int conn, InetAddr& peer);
    void removeConnectionInLoop(const ConnectionPtr conn);

    ThreadInitCallback threadInitCallback_;
    newConnCallback newConnCallback_;
    msgOutCallback msgOutCallback_;

    // ReadCallback readCallback_;
    // WriteCallback writeCallback_;
    // CloseCallback closeCallback_;

};

}


#endif