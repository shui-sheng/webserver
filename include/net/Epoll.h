#ifndef _EPOLL_H_
#define _EPOLL_H_

#include"sys/epoll.h"
#include"Channel.h"
#include"Error.h"
#include<stdio.h>

#include<vector>
#include<map>

namespace fei
{
class Channel;
class Epoll
{
public:
    typedef std::vector<struct epoll_event> eventList;
    typedef std::vector<Channel*> ChannelList;

    Epoll();
    ~Epoll();
    void epoll(ChannelList* activeChannels);
    void update(Channel* ch);
    int fd() { return epollfd_; }
    //void remove(Channel* ch);
private:

    int epollfd_;
    std::map<int, Channel*> map_;
    eventList eventList_;

};


}


#endif