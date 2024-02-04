#include"../../include/net/EventLoopThreadPool.h"

using namespace fei;


EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
    :baseloop_(baseLoop)
    ,started_(false)
    ,numThreads_(0)
    ,next_(0)
    {

    }
// typedef std::function<void(EventLoop*)> ThreadEventInitCallback;

void EventLoopThreadPool::start(const ThreadEventInitCallback& cb)
{
    baseloop_->assertInLoopThread();
    started_ = true;

    for(int i = 0; i < numThreads_; ++i)
    {
        EventLoopThread* t = new EventLoopThread(cb);
        threads_.push_back(t);
        loops_.push_back(t->startLoop());
    }
}

EventLoop* EventLoopThreadPool::getNextLoop()
{
    baseloop_->assertInLoopThread();

    EventLoop* ret = baseloop_;
    if(!loops_.empty())
    {
        ret = loops_[next_++];
        if((size_t)next_ >= loops_.size())
        {
            next_ = 0;
        }
    }
    return ret;
}