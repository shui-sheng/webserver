#include"../../include/thread/ThreadPool.h"

using namespace fei;

ThreadPool::ThreadPool(const std::string& name)
    :mutex_()
    ,notEmpty_(mutex_)
    ,notFull_(mutex_)
    ,name_(name)
    ,maxQueueSize_(0)
    ,running_(false)
{   

}

ThreadPool::~ThreadPool()
{
    if(running_)
    {
        stop();
    }
}

void ThreadPool::start(int numThreads)
{
    running_ = true;
    threads_.reserve(numThreads);

    for(int i = 0; i < numThreads; i++)
    {
        char id[32];
        snprintf(id, sizeof(id), "%d", i+1);
        threads_.emplace_back(new Thread(std::bind(&ThreadPool::runInThread, this), name_+id));
        threads_[i]->start();
        
    }
    //sleep(1);

}

void ThreadPool::stop()
{
    {
        MutexLockGuard lock(mutex_);
        running_ = false;
        notEmpty_.nodifyAll();
        notFull_.nodifyAll();
    }
    for (auto& thr : threads_)
    {
        thr->join();
    }
}
void ThreadPool::run(Task task)//add a new task into the task_queue
{
    // if(threads_.empty())
    // {
    //     task();
    // }

        MutexLockGuard lock(mutex_);
        while(isFull() && running_)
        {
            notFull_.wait();
        }
        if(!running_) return;
        assert(!isFull());

        queue_.push_back(std::move(task));
        notEmpty_.nodify();


    
}

ThreadPool::Task ThreadPool::take()
{
    MutexLockGuard lock(mutex_);
    while(queue_.empty() && running_)
    {
        notEmpty_.wait();
    }
    Task task;
    if(!queue_.empty())
    {
        task = queue_.front();
        queue_.pop_front();
        if(maxQueueSize_ > 0)
        {
            notFull_.nodify();
        }
    }
    return task;
}
bool ThreadPool::isFull()
{
    //not locked, must ensure the caller function has locked the mutex_
    return queue_.size() >= maxQueueSize_;
}

void ThreadPool::runInThread()
{

    while(running_)
    {
        Task task(take());
        if(task)
            task();    
        




    }
}