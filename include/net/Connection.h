#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include"../log/Logger.h"
#include"Channel.h"
#include"InetAddr.h"
#include"Socket.h"
#include"Buffer.h"
#include"HttpContext.h"
namespace fei
{

class Connection
{
public:
    typedef Connection* ConnectionPtr;
    typedef std::function<void(int connfd, InetAddr& peerAddr)> msgInCallback;
    typedef std::function<void(const ConnectionPtr& conn, Buffer& buffer , const Timestamp& now)> msgOutCallback;
    typedef std::function<void(const ConnectionPtr& conn)> CloseCallback;
public:
    Connection(EventLoop* loop, int acceptfd, InetAddr& local, InetAddr& peer, std::string name);

    void setMsgInCallback(const msgInCallback& cb) { msgInCallback_ = cb; }
    void setMsgOutCallback(const msgOutCallback& cb) { msgOutCallback_ = cb; }
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; }

    void send(const char* str);
    void send(const std::string& str);
    void send(char* data, size_t len);
    void sendInLoop(const char* data, size_t len);
    // {
    //     loop_->assertInLoopThread();
    //     ssize_t nwrote = write(connectSocket_->Fd(), data, len);
    //     LOG_INFO<< "Write " << nwrote << " Bytes";
    // }
    //void setErrorCallback(const ErrorCallback& cb) { errorCallback_ = cb; }

    void established();

    std::string name(){ return name_; }
private:


    EventLoop* loop_;
    Socket* connectSocket_;
    Channel* connectChannel_;
    InetAddr localAddr_;
    InetAddr peerAddr_;
    std::string name_;
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();

    msgInCallback msgInCallback_;
    msgOutCallback msgOutCallback_;

    CloseCallback closeCallback_;

    Buffer inbuffer_;
    Buffer outbuffer_;
    HttpContext context_;

    


};


}


#endif