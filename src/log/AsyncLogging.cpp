#include"../../include/log/AsyncLogging.h"
#include"../../include/log/LogFile.h"
#include <sys/types.h>
#include <sys/stat.h>

#include"fcntl.h"
using namespace fei;

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

}


void AsyncLogging::append(const char* msg, size_t len)
{
    MutexLockGuard lock(mutex_);

    if(currentBuffer_->avail() > len)
    {
        currentBuffer_->append(msg, len);
    }   
    else 
    {
        buffers_.push_back(std::move(currentBuffer_));
        if(nextBuffer_)
        {
            currentBuffer_ = std::move(nextBuffer_);

        }
        else 
        {
            currentBuffer_.reset(new Buffer);
        }

        currentBuffer_->append(msg, len);
    
        cond_.nodify();
    }
}


void AsyncLogging::ThreadFunc()
{
    //static int cnt = 0;
    assert(running_ == true);
    latch_.countDown();
    LogFile outputfile(basename_, rollSize_, false);
    BufferPtr buffer1(new Buffer);
    BufferPtr buffer2(new Buffer);
    buffer1->bzero();
    buffer2->bzero();

    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);

    while(running_)
    {
        //cnt++;
        //printf("over-1\n");
        assert(buffer1);
        assert(buffer2);
        //printf("%d %d\n", buffer1->length(), buffer2->length());
        assert(buffer1 && buffer1->length() == 0);
        assert(buffer2 && buffer2->length() == 0);
        //printf("over0\n");
        assert(buffersToWrite.empty());
        //printf("over1\n");
        

        {
            //把currentbuffer加入buffers,并用预先储备的buf1替换
            //交换buffersToWrite
            //若nextbuffer空，则用buf2替换
            MutexLockGuard lock(mutex_);
            //
            if(buffers_.empty())
            {
                cond_.waitForSeconds(4);
            }
            buffers_.push_back(std::move(currentBuffer_));
            currentBuffer_ = std::move(buffer1);

            buffersToWrite.swap(buffers_);
            if(!nextBuffer_) 
                nextBuffer_ = std::move(buffer2);
            
            assert(!buffersToWrite.empty());
            
        }


        for(auto &buf : buffersToWrite)
        {

            outputfile.append(buf->data(), buf->length());
        }

        if(buffersToWrite.size() > 2)
        {
            buffersToWrite.resize(2);
        }
        //如果buf1被替换上currentbuf
        if(!buffer1)
        {
            assert(!buffersToWrite.empty());

            buffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            buffer1->bzero();
        }
        if(!buffer2)
        {
            assert(!buffersToWrite.empty());
            buffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            buffer2->bzero();
        }
        buffersToWrite.clear();
        outputfile.flush();
    }
    outputfile.flush();



}