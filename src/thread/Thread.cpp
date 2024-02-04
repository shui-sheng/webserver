#include"../../include/thread/Thread.h"
#include"../../include/thread/CurrentThread.h"



#include<unistd.h>
#include<sys/types.h>
#include<sys/prctl.h>
#include<assert.h>
#include<time.h>
#include<cstring>
namespace fei
{


void afterFork()
{
    CurrentThread::t_cachedTid = gettid();
    CurrentThread::t_threadName = "main";
}

class ThreadNameInitializer
{
public:
    ThreadNameInitializer()
    {
        CurrentThread::t_threadName = "main";
        pthread_atfork(NULL, NULL, &afterFork);
    }

};

ThreadNameInitializer init;

struct ThreadData
{
    typedef fei::Thread::ThreadFunc ThreadFunc;
    
    ThreadFunc func_;
    std::string name_;

    pid_t* tid_;
    fei::CountDownLatch* latch_;
    ThreadData(const ThreadFunc& func
              ,const std::string& name
              ,pid_t* tid
              ,fei::CountDownLatch* latch)
        :func_(func), name_(name), tid_(tid),latch_(latch)
    {
    }
    void runInThread()
    {
        //this->tid_ == Thread.tid_ == &CurrentThread.t_cachedTid

        *tid_ = fei::CurrentThread::tid();
        tid_ = NULL; //No longer needed
        
        latch_->countDown();//after setting tid_, Thread::start() can go on

        if(!name_.empty())
            CurrentThread::t_threadName = name_.c_str();

        ::prctl(PR_SET_NAME, fei::CurrentThread::t_threadName);

        //func_();
        // try {
            func_();
        // } catch (const std::bad_function_call& e) {
        //     if(func_)
        //         std::cerr<<"func_ isnot null\n";
        //     else std::cerr<<"func_ not null\n";
        //     std::cerr<<"Currenttid="<<CurrentThread::tid();
        //     std::cerr << "-Caught exception: " << e.what() << std::endl;

        // }
        CurrentThread::t_threadName = "finished";
    }
};

//global func
void* startThread(void* obj)
{
    
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}
}


using namespace fei;



AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const std::string& name)
    :started_(false)
    ,joined_(false)
    ,pthreadId_(0)
    ,tid_(new pid_t(0))
    ,func_(func)
    ,name_(name)
    ,latch_(1)
{
    numCreated_.incrementAndGet();
}

Thread::~Thread()
{
    if(started_ && !joined_)
        pthread_detach(pthreadId_);
}

void Thread::start()
{
    assert(!started_);
    started_ = true;

    ThreadData* data = new ThreadData(func_, name_, tid_, &latch_);
    if(pthread_create(&pthreadId_, NULL, &startThread, data))
    {
        started_ = false;
        delete data;
        abort();
    }
    else
    {
        latch_.wait();
        assert(tid_ > 0);
    }
}

void Thread::join()
{
    assert(started_ && !joined_);

    joined_ = true;


    // struct timespec ts;
    // if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
    //     assert(0);
    // }
    // ts.tv_sec += 5;
    // int ret = pthread_timedjoin_np(pthreadId_, NULL, &ts);
    // if (ret != 0) {
    //     std::cerr<<strerror(ret)<<std::endl;
    //     assert(0);
    // }

    pthread_join(pthreadId_, NULL);

}
