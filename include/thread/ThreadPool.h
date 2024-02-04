#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_
#include<functional>
#include<string>
#include"../thread/Condition.h"
#include"../thread/Mutex.h"
#include"../thread/Thread.h"
#include<vector>
#include<deque>
#include<memory>
namespace fei
{

class ThreadPool
{

public:
    typedef std::function<void()> Task;

    explicit ThreadPool(const std::string& name = std::string("ThreadPool"));
    ~ThreadPool();

    void setMaxQueueSize(int maxSize) 
    {
        assert(maxSize != 0);
        maxQueueSize_ = maxSize;
    }
    // void setThreadInitCallback(const Task& cb)
    // {   threadInitCallback_ = cb; }
    
    void start(int numThreads);
    void stop();

    bool isFull();
    void run(Task task);
    Task take();

private:    
    void runInThread();


    MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
    std::string name_;
    //Task threadInitCallback_;
    std::vector<std::unique_ptr<Thread>> threads_;
    std::deque<Task> queue_;
    size_t maxQueueSize_;
    bool running_;

};

}

#endif