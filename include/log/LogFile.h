#ifndef _LOGFILE_H_
#define _LOGFILE_H_
#include<string>
#include<memory>
#include"../thread/Mutex.h"


namespace fei
{
namespace FileUtil
{
class AppendFile ;
}


class LogFile
{
public:
    LogFile(const std::string& basename,
            off_t rollSize,
            bool threadSafe = true,
            int flushInterval = 3,
            int checkEveryN = 1024);
    ~LogFile();

    void append(const char* logline, size_t len);

    void flush();
    bool rollFile();

private:


    void append_unlocked(const char* logline, size_t len);


    static std::string getLogFileName(const std::string& basename, time_t* now);

    const std::string basename_;
    const off_t rollSize_;
    const int flushInterval_;
    const int checkEveryN_;

    int count_;

    std::unique_ptr<MutexLock> mutex_;
    time_t startOfPeriod_;
    time_t lastRoll_;
    time_t lastFlush_;
    std::unique_ptr<FileUtil::AppendFile> file_;

    const static int kRollPerSeconds_ = 60*60*24;
};
}
#endif