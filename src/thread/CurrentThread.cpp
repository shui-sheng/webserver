#include"../../include/thread/CurrentThread.h"
namespace fei
{
namespace CurrentThread
{
    __thread const char* t_threadName = "unnamedthread";
    __thread pid_t t_cachedTid = 0;
}
}

