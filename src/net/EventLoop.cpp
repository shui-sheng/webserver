#include"../../include/net/EventLoop.h"
#include"../../include/time/TimerQueue.h"
#include"../../include/log/Logger.h"
#include<sys/eventfd.h>
#include<signal.h>


using namespace fei;

__thread EventLoop* t_loopInThisThread = 0;

class IgnoreSigPipe
{
 public:
  IgnoreSigPipe()
  {
    ::signal(SIGPIPE, SIG_IGN);
  }
};

IgnoreSigPipe initObj;
static int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(evtfd < 0)
    {
        abort();
    }
    return evtfd;
}
/*
bool callingPendingFunctors_;
    int wakeupFd_;
    Channel* wakeupChannel_;
    bool quit_;

    Epoll* ep_;
    const pid_t threadId_;
    ChannelList activeChannels_;
    MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;
    TimerQueue* timerQueue_;*/
EventLoop::EventLoop()
    :wakeupFd_(createEventfd())
    ,wakeupChannel_(new Channel(this, wakeupFd_))
    ,quit_(false)
    ,ep_(new Epoll())
    ,threadId_(CurrentThread::tid())
    //,wakeupFd_(createEventfd())
    ,callingPendingFunctors_(false)
    ,timerQueue_(new TimerQueue(this))

{
    assert(ep_ != NULL);
    //printf("EventLoop created %x in thread %d \n", this, threadId_);
    if(t_loopInThisThread)
    {
        LOG_FATAL<<"Another EventLoop "<< t_loopInThisThread << " has existed in this thread " << threadId_;
        //printf("Another EventLoop %x has existed in this thread\n", t_loopInThisThread, threadId_);
    }
    else 
    {
        t_loopInThisThread = this;
    }
    LOG_INFO <<"EventLoop created " << this << " in thread " << threadId_ 
             <<" wakeup_fd = " << wakeupFd_  ;

    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead,this));
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
    ::close(wakeupFd_);
    t_loopInThisThread = NULL;   
}
void EventLoop::update(Channel* ch)
{
    assertInLoopThread();
    assert(ch->loop() == this);
    ep_->update(ch);
}
void EventLoop::quit()
{
    quit_ = true;

    if(!isInLoopThread())
        wakeup();
}
void EventLoop::loop()
{
    assertInLoopThread();
    while(!quit_)
    {
        activeChannels_.clear();
        ep_->epoll(&activeChannels_);
        //printf("%s:epoll over\n");
        for(int i = 0; i < activeChannels_.size(); i++)
        {
            activeChannels_[i]->handle();
        }
        dopendingFunctors();

    }

}

void EventLoop::abortNotInLoopThread()
{
    printf("%s: EventLoop was created in threadId_=%d, current thread id = %d\n",
         __func__, threadId_, CurrentThread::tid());
    abort();
}

void EventLoop::runInLoop(const Functor& cb)
{
    if(isInLoopThread())
        cb();
    else 
        queueInLoop(cb);
}
void EventLoop::queueInLoop(const Functor& cb)
{
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(cb);
    }
    if(!isInLoopThread() || callingPendingFunctors_)
    {
        wakeup();
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(wakeupFd_, &one, sizeof(one));
    assert(n == sizeof(one));
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = ::read(wakeupFd_, &one, sizeof(one));
    assert(n == sizeof(one));
}
void EventLoop::dopendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(size_t i = 0; i < functors.size(); ++i)
        functors[i]();
    callingPendingFunctors_ = false;
}

TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
    return timerQueue_->addTimer(cb, time, 0.0);
}
TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
{
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, cb);
}
TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
{
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(cb, time, interval);
}
