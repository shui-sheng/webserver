#include "../../include/net/Epoll.h"
#include "../../include/log/Logger.h"
using namespace fei;

Epoll::Epoll()
    : epollfd_(epoll_create(1024))
{
    eventList_.resize(100);
    LOG_INFO << "Epoll_fd = " << epollfd_;
    perror_if(eventList_.size() != 100, "Epoll::resize()\n");
    perror_if(epollfd_ < 0, "Epoll::EPoll()\n");
}

Epoll::~Epoll()
{
}

void Epoll::epoll(ChannelList *activeChannels)
{
    int numEvents = epoll_wait(epollfd_, &*eventList_.begin(), eventList_.size(), -1);
    for (int i = 0; i < numEvents; ++i)
    {
        Channel *ch = (Channel *)eventList_[i].data.ptr;
        if (eventList_[i].events & EPOLLIN)
        {
            assert(ch->isReading());
            ch->setRevent(EPOLLIN | EPOLLET);
        }
        if (eventList_[i].events & EPOLLOUT)
        {
            ch->setRevent(EPOLLOUT);
        }
        // if(eventList_[i].events & EPOLLIN) ch->setRevent(EPOLLIN);

        activeChannels->push_back(ch);
    }
}

void Epoll::update(Channel *ch)
{
    assert(ch != nullptr);
    if (ch->event() == 0)
    {
        // LOG_INFO << "update del fd " << ch->fd()
        //          << " event = " << (ch->isReading() ? "EPOLLIN" : 0) << (ch->isWriting() ? "EPOLLIOUT" : 0)
        //          << " revent = " << ch->revent();
        // std::cout<<ch->event()<<std::endl;
        epoll_ctl(epollfd_, EPOLL_CTL_DEL, ch->fd(), NULL);
        map_.erase(ch->fd());
        return;
    }
    // std::cout<<ch->event()<<std::endl;

    epoll_event ev;
    ev.data.ptr = ch;
    ev.events = ch->event();
    // LOG_INFO <<"update fd " << ch->fd()
    //          <<" event = " << (epoll_event)
    //          <<" revent = " << ch->revent() ;

    // printf("%s: Fd = %d\n", __func__,ch->fd());
    // add
    if (map_.find(ch->fd()) == map_.end())
    {
        // LOG_INFO << "update add fd " << ch->fd()
        //          << " event = " << (ch->isReading() ? "READ" : 0) << (ch->isWriting() ? " WRITE" : "")
        //          << " revent = " << ch->revent();
        map_.insert(std::pair<int, Channel *>(ch->fd(), ch));
        epoll_ctl(epollfd_, EPOLL_CTL_ADD, ch->fd(), &ev);
    }
    else // mod
    {
        // LOG_INFO << "update mod fd " << ch->fd()
        //          << " event = " << (ch->isReading() ? "EPOLLIN" : 0) << (ch->isWriting() ? "EPOLLIOUT" : 0)
        //          << " revent = " << ch->revent();
        map_[ch->fd()] = ch;
        epoll_ctl(epollfd_, EPOLL_CTL_MOD, ch->fd(), &ev);
    }
}