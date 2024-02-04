#include"../../include/net/Channel.h"
#include"../../include/log/Logger.h"
using namespace fei;

Channel::Channel(EventLoop* loop, int fd)
    :loop_(loop), fd_(fd)
    ,events_(0),revents_(0)
    ,idx_(-1)
{

}

void Channel::removeAll()
{
    events_ = 0;
    update();
}
void Channel::removeReading()
{
    events_ &= ~EPOLLIN;
    update();
}
void Channel::removeWriting()
{
    events_ &= ~EPOLLOUT;
    update();
}
void Channel::enableAll()
{
    events_ |= (EPOLLIN | EPOLLOUT );
    update();

}
void Channel::enableReading()
{
    events_ |= (EPOLLIN | EPOLLHUP);
    update();
}
void Channel::enableWriting()
{
    events_ |= (EPOLLOUT | EPOLLHUP);
    update();
}
void Channel::update()
{
    loop_->update(this);
}
void Channel::handle()
{
    //printf("%s\n", __func__);
    if(revents_ & EPOLLHUP)
    {
        if(closeCallback_)
        {
            closeCallback_();
        }
    }
    if(revents_ & (EPOLLIN))
    {
        //printf("%d's revents read\n", fd_);
        if(readCallback_)
            readCallback_();
    }
    if(revents_ &(EPOLLOUT))
    {
        //printf("%d's revents write\n", fd_);
        if(writeCallback_)
            writeCallback_();
    }
    
}