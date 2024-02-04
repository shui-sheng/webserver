#ifndef _ASYNCLOGGING_H_
#define _ASYNCLOGGING_H_

#include<string>
#include"../thread/Mutex.h"
#include"../thread/Thread.h"
#include"../thread/Condition.h"
#include"../thread/CountDownLatch.h"
#include"LogStream.h"
#include<vector>
#include<atomic>
namespace fei
{

/*
AsyncLogging::AsyncLogging(const std::string& basename
                          ,off_t rollSize
                          ,int flushInterval)
    :cond_(mutex_)
    ,thread_(std::bind(&AsyncLogging::ThreadFunc, this), "LogThread")
    ,currentBuffer_(new Buffer)
    ,nextBuffer_(new Buffer)
    ,running_(false)
    ,latch_(1)
    ,basename_(basename)
    ,flushInterval_(flushInterval)
    ,rollSize_(rollSize)
{
    currentBuffer_->bzero();
    nextBuffer_->bzero();
    buffers_.reserve(16);

}*/
class AsyncLogging
{
public:
    AsyncLogging(const std::string& basename
                          ,off_t rollSize
                          ,int flushInterval);

    ~AsyncLogging()
    {
        if (running_)
        {
        stop();
        }
    }

    void start()
    {
        running_ = true;
        thread_.start();
        latch_.wait();
    }
    void stop() 
    {
        running_ = false;
        cond_.nodify();
        thread_.join();
    }
    void append(const char* msg, size_t len);

private:

    void ThreadFunc();

    typedef FixedBuffer<kLargeBuffer> Buffer;

    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    const off_t rollSize_;

    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    //std::vector<std::unique_ptr<Buffer>>;
    std::vector<std::unique_ptr<Buffer>> buffers_;
    MutexLock mutex_;
    Condition cond_;
    Thread thread_;
    std::atomic<bool> running_;
    CountDownLatch latch_;
    const std::string basename_;
    const int flushInterval_;


    

};
}








#endif