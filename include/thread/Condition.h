#ifndef _CONDITION_H_
#define _CONDITION_H_


#include"Mutex.h"
#include<pthread.h>
#include<errno.h>

namespace fei
{

class Condition
{
public:
    explicit Condition(MutexLock& mutex)
        :mutex_(mutex)
    {
        pthread_cond_init(&pcond_, NULL);
    }
    ~Condition()
    {
        pthread_cond_destroy(&pcond_);
    }
    void wait()
    {
        pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
    }

    bool waitForSeconds(int seconds)
    {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += seconds;
        return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
    }

    void nodify() { pthread_cond_signal(&pcond_); }
    void nodifyAll() { pthread_cond_broadcast(&pcond_); }
    

private:
    MutexLock& mutex_;
    pthread_cond_t pcond_;
};



}


#endif