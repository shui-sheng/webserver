#ifndef _COUNTDOWNLATCH_H_
#define _COUNTDOWNLATCH_H_

#include"Mutex.h"
#include"Condition.h"

namespace fei
{
class CountDownLatch 
{

public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();
    int getCount();


private:
    MutexLock mutex_;
    Condition condition_;
    int count_;

};
}

#endif