#ifndef _THREAD_H_
#define _THREAD_H_

#include"Atomic.h"
#include"CountDownLatch.h"

#include<pthread.h>
#include<functional>
#include<iostream>
#include<memory>

namespace fei
{

class Thread
{
public:

    typedef std::function<void()> ThreadFunc;

    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();

    void start();
    void join();

    bool started() const { return started_; }

    pid_t tid() const { return *tid_; }
    const std::string& name() const { return name_; }
    CountDownLatch latch_;
    static int numCreated() { return numCreated_.get(); }


private:
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t* tid_;
    ThreadFunc func_;
    std::string name_;

    static AtomicInt32 numCreated_;
};



}



#endif


