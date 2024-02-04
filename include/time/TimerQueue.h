#ifndef _TIMERQUEUE_H
#define _TIMERQUEUE_H


#include<set>
#include<vector>

#include"../thread/Mutex.h"
#include"../net/CallBacks.h"
#include"../net/Channel.h"

#include"../net/Error.h"
namespace fei
{
class EventLoop;
class Timer;
class TimerId;

class TimerQueue
{
public: 
    TimerQueue(EventLoop* loop);
    ~TimerQueue();

    TimerId addTimer(const TimerCallback& cb, Timestamp when, double interval);
private:
    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;//has sorted by Timestamp

    void addTimerInLoop(Timer* timer);
    void handleRead();
    
    std::vector<Entry> getExpired(Timestamp now);

    void reset(const std::vector<Entry>& expired, Timestamp now);
    bool insert(Timer* timer);

    EventLoop* loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    TimerList timers_;

};
}

#endif