#ifndef _CURRENTTHREAD_H_
#define _CURRENTTHREAD_H_

#include<sys/types.h>
#include<unistd.h>
#include<pthread.h>

namespace fei
{
namespace CurrentThread
{
    extern __thread const char* t_threadName;
    extern __thread pid_t t_cachedTid;

    inline int tid()
    {
        if(t_cachedTid == 0)
            t_cachedTid = gettid();
        return t_cachedTid;
    }

    inline const char* name()
    {
        return t_threadName;
    }
    inline bool isMainThread()
    {
        return tid() == ::getpid();
    }




}
}

#endif