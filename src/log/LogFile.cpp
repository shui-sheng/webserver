
#include"../../include/log/LogFile.h"
#include"../../include/log/FileUtil.h"
#include<iostream>
#include<assert.h>
#include<stdio.h>
#include<time.h>
#include<sys/types.h>
#include<unistd.h>
using namespace fei;

LogFile::LogFile(const std::string& basename,
                off_t rollSize,
                bool threadSafe,
                int flushInterval,
                int checkEveryN)
    :basename_(basename),
    rollSize_(rollSize),
    flushInterval_(flushInterval),
    checkEveryN_(checkEveryN),
    count_(0),
    mutex_(threadSafe ? new MutexLock : NULL),
    startOfPeriod_(0),
    lastRoll_(0),
    lastFlush_(0)
{
    assert(basename.find('/') == std::string::npos);
    rollFile();
}

LogFile::~LogFile() = default;

void LogFile::append(const char* logline, size_t len)
{
    if(mutex_)
    {
        MutexLockGuard lock(*mutex_);
        append_unlocked(logline, len);
    }
    else 
    {
        append_unlocked(logline, len);
    }
}

void LogFile::flush()
{
    if(mutex_)
    {
        MutexLockGuard lock(*mutex_);
        file_->flush();
    }
    else
    {
        file_->flush();
    }
}

void LogFile::append_unlocked(const char* logline, size_t len)
{
    file_->append(logline, len);
    if(file_->writtenBytes() > rollSize_)
    {
        rollFile();
    }
    else
    {
        ++count_;
        if(count_ > checkEveryN_)
        {
            time_t now = ::time(NULL);
            time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
            if(thisPeriod_ != startOfPeriod_)
            {
                rollFile();
            }
            else if(now - lastFlush_ > flushInterval_)
            {
                lastFlush_ = now;
                file_->flush();
            }
        }

    }
}

bool LogFile::rollFile()
{
    time_t now = 0;
    std::string filename = getLogFileName(basename_, &now);
    time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;
    if(now > lastRoll_)
    {
        lastRoll_ = now;
        lastFlush_ = now;
        startOfPeriod_ = start;
        file_.reset(new FileUtil::AppendFile(filename.c_str()));
        return true;
    }
    return false;
}


std::string LogFile::getLogFileName(const std::string& basename, time_t* now)
{
    std::string filename;
    filename.reserve(basename.size() + 64);
    filename = basename;

    char timebuf[32];
    struct tm tm;
    *now = time(NULL);

    gmtime_r(now, &tm);
    strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
    filename += timebuf;
    
    char pidbuf[32];
    snprintf(pidbuf, sizeof(pidbuf), ".%d", ::getpid());
    filename += pidbuf;
    filename += ".log";
    //std::cout<<filename<<std::endl;
    return filename;

}
