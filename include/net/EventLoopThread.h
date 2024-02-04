#ifndef _EVENTLOOPTHREAD_H_
#define _EVENTLOOPTHREAD_H_


#include"../thread/Condition.h"
#include"../thread/Mutex.h"
#include"../thread/Thread.h"

namespace fei
{
class EventLoop;

class EventLoopThread
{

public:
    typedef std::function<void(EventLoop*)> ThreadEventInitCallback;
    EventLoopThread(const ThreadEventInitCallback& cb = ThreadEventInitCallback());

    ~EventLoopThread();
    EventLoop* startLoop();
private:
    void threadFunc();
    ThreadEventInitCallback threadEventInitCallback_;
    EventLoop* loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};

}

#endif