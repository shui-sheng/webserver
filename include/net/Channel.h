#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include"CallBacks.h"
#include"EventLoop.h"
#include<sys/epoll.h>
namespace fei
{
class EventLoop;
class Channel
{
public:
    //typedef ReadingEvents EPOLLIN;
    enum class EventType {READ, WRITE, CLOSE};
    Channel(EventLoop* loop, int fd);
    //~Channel();



    int  fd() { return fd_; }
    unsigned int  event() { return events_; }
    unsigned int  revent() { return revents_; }
    void setEvent(unsigned int event) { events_ |= event; }
    void setRevent(unsigned int revent) { revents_ |=  revent; }
    bool isReading() { return events_ & EPOLLIN; }
    bool isWriting() { return events_ & EPOLLOUT; }

    void handle();

    void removeAll();
    void removeReading();
    void removeWriting();
    void enableAll();
    void enableReading();
    void enableWriting();
    void update();

    void setReadCallback(const ReadCallback& cb) { readCallback_ = cb;}
    void setWriteCallback(const WriteCallback& cb) { writeCallback_ = cb; }
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; }
    void setErrorCallback(const ErrorCallback& cb) { errorCallback_ = cb; }


    EventLoop* loop() { return loop_; }
private:
    EventLoop* loop_;

    int fd_;
    unsigned int events_;
    unsigned int revents_;
    int idx_;
    
    ReadCallback readCallback_;
    WriteCallback writeCallback_;
    CloseCallback closeCallback_;
    ErrorCallback errorCallback_;
};

} // namespace fei





#endif