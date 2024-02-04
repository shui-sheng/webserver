

#include"../../include/time/TimerQueue.h"
#include"../../include/time/Timer.h"
#include"../../include/time/TimerId.h"
#include"../../include/time/Timestamp.h"
#include"../../include/log/Logger.h"
#include"../../include/net/Error.h"
#include<sys/timerfd.h>
#include<stdio.h>
#include<time.h>
#include<strings.h>
namespace fei
{

int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    perror_if(timerfd < 0, "createTimerfd()");
    return timerfd;
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
    int64_t microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
    if(microseconds < 100)
    {
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>((microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
    return ts;
}

void readTimerfd(int timerfd, Timestamp now)
{
    uint64_t howmany;
    ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
    printf("%s:TimerQueue::handleRead() %ld at %s\n", __func__, howmany, now.toString().c_str());

    assert(n == sizeof(howmany));
}

void resetTimerfd(int timerfd, Timestamp expiration)
{
    struct itimerspec newvalue;
    struct itimerspec oldvalue;
    bzero(&newvalue, sizeof(newvalue));
    bzero(&oldvalue, sizeof(oldvalue));
    newvalue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd, 0, &newvalue, &oldvalue);
    assert(!ret);

}


}

using namespace fei;

//runAt->


//Init timerfd, timerfdChannel
/*
   |addTimer——|
   |          |addTimerInLoop——|
   |                           |insert
   |                           |if(earliestchanged),resetTimerfd
   |
   |handleRead——|
   |            |readTimerfd
   |            |getExpired and erase, run
   |            |reset——|
   |                    |addrepeatedTimer
   |                    |find the first timer and resetTimerfd
   |
   |
   |
*/
TimerQueue::TimerQueue(EventLoop* loop)
    :loop_(loop)
    ,timerfd_(createTimerfd())
    ,timerfdChannel_(loop, timerfd_)
    ,timers_()
{
    timerfdChannel_.setReadCallback(std::bind(&TimerQueue::handleRead, this));
    timerfdChannel_.enableReading();
    LOG_INFO<< "Timer_fd = " << timerfd_;
}


TimerQueue::~TimerQueue()
{
    ::close(timerfd_);
    for(auto it = timers_.begin(); it != timers_.end(); it++)
        delete it->second;
}
TimerId TimerQueue::addTimer(const TimerCallback& cb, Timestamp when, double interval)
{
    Timer* timer = new Timer(cb, when, interval);
    loop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
    return TimerId(timer);
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
    loop_->assertInLoopThread();
    bool earliestChanged = insert(timer);
    if(earliestChanged)
        resetTimerfd(timerfd_, timer->expiration());
}

void TimerQueue::handleRead()
{
    loop_->assertInLoopThread();
    Timestamp now(Timestamp::now());

    readTimerfd(timerfd_, now);
    std::vector<Entry> expired = getExpired(now);

    for(auto it = expired.begin(); it != expired.end(); it++)
    {
        it->second->run();
    }

    reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
    std::vector<Entry> expired;
    Entry sentry = std::make_pair(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    TimerList::iterator it = timers_.lower_bound(sentry);
    assert(it == timers_.end() || now < it->first);
    std::copy(timers_.begin(), it, back_inserter(expired));
    timers_.erase(timers_.begin(), it);

    return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
    Timestamp nextExpire;
    for(auto it = expired.begin(); it != expired.end(); it++)
    {
        if(it->second->repeat())
        {
            it->second->restart(now);
            insert(it->second);
        }
        else 
        {
            delete it->second;
        }
    }
    if(!timers_.empty())
    {
        nextExpire = timers_.begin()->second->expiration();
    }
    if(nextExpire.valid())
    {
        resetTimerfd(timerfd_, nextExpire);
    }
}

bool TimerQueue::insert(Timer* timer)
{
    bool earliestChanged = false;
    Timestamp when  = timer->expiration();
    auto it = timers_.begin();
    if(it == timers_.end() || when < it->first)
        earliestChanged = true;
    std::pair<TimerList::iterator, bool> ret = 
        timers_.insert(std::make_pair(when, timer));
    assert(ret.second);
    return earliestChanged;
}

