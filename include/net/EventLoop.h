#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include"Error.h"
#include"Epoll.h"
#include"../thread/Thread.h"
#include"../thread/Mutex.h"
#include"Channel.h"
#include"../time/TimerId.h"
#include"../time/Timestamp.h"

#include<unistd.h>
#include<vector>

namespace fei
{
class Epoll;
class Channel;
class TimerQueue;


class EventLoop
{
public:
    typedef std::function<void()> Functor;

    EventLoop();
    ~EventLoop();

    void update(Channel* ch);
    void loop();
    void quit();
    TimerId runAt(const Timestamp& time, const TimerCallback& cb);
    TimerId runAfter(double delay, const TimerCallback& cb);
    TimerId runEvery(double interval, const TimerCallback& cb);

    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
    void assertInLoopThread()
    {
        //printf("%s:threadID=%d, currentTid=%d\n", __func__, threadId_, CurrentThread::tid());
        if(!isInLoopThread())
            abortNotInLoopThread();
    }
    void runInLoop(const Functor& cb);
    void queueInLoop(const Functor& cb);
    void wakeup();

private:    
    typedef std::vector<Channel*> ChannelList;
    
    void abortNotInLoopThread();
    void handleRead();
    void dopendingFunctors();

    bool callingPendingFunctors_;
    int wakeupFd_;
    Channel* wakeupChannel_;
    bool quit_;

    Epoll* ep_;
    const pid_t threadId_;
    ChannelList activeChannels_;
    MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;
    TimerQueue* timerQueue_;
    

};




}

#endif