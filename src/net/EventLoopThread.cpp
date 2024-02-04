#include"../../include/net/EventLoopThread.h"
#include"../../include/net/EventLoop.h"

using namespace fei;
EventLoopThread::EventLoopThread(const ThreadEventInitCallback& cb)
    :loop_(NULL)
    ,exiting_(false)
    ,thread_(std::bind(&EventLoopThread::threadFunc,this))
    ,mutex_()
    ,cond_(mutex_)
    ,threadEventInitCallback_(cb)
{   

}


EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    loop_->quit();
    thread_.join();
}

EventLoop* EventLoopThread::startLoop()
{
    assert(!thread_.started());
    thread_.start();

    {
        MutexLockGuard lock(mutex_);
        while(loop_ == NULL)
            cond_.wait();
    }
    return loop_;

}

void EventLoopThread::threadFunc()
{
    //typedef std::function<void(EventLoop*)> ThreadEventInitCallback;
    //


    EventLoop loop;
    
    if(threadEventInitCallback_)
        threadEventInitCallback_(&loop);

    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        cond_.nodify();
    }
    loop.loop();
}
