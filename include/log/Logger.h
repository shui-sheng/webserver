#ifndef _LOGGER_H_
#define _LOGGER_H_

#include"LogStream.h"
#include"../time/Timestamp.h"

namespace fei
{
/*
Logger::Logger(const char* file, int line, LogLevel level,const char* func)
    :impl_(file, line, level, func)
{
    //printf("Logger:%s,%d,%s\n", file, line, func);
}*/
class Logger
{
 public:
    enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };
    typedef void (*OutputFunc)(const char* msg, size_t len);
    typedef void (*FlushFunc)();

public:

    Logger(const char* file, int line, LogLevel level,const char* func);
    ~Logger();
    LogStream& stream() { return impl_.stream_; }

    //static void setOutput(OutputFunc);

    
    static void setOutputFunc(OutputFunc f);
    //static void setFlushFunc(FlushFunc f);

private:

    class Impl
    {
    public:
        typedef Logger::LogLevel LogLevel;

        Impl(const char* file, int line, LogLevel level,const char* func);
        void finish();

        const char* file_;
        int line_;
        LogLevel level_;
        const char* func_;

        std::string basename_ = "basename";
        Timestamp time_;
        LogStream stream_;
       //
    };
    Impl impl_;

};
}

#define LOG_INFO  fei::Logger(__FILE__, __LINE__, fei::Logger::INFO, __func__).stream()
#define LOG_WARN  fei::Logger(__FILE__, __LINE__, fei::Logger::WARN, __func__).stream()
#define LOG_ERROR  fei::Logger(__FILE__, __LINE__, fei::Logger::ERROR, __func__).stream()
#define LOG_WARN  fei::Logger(__FILE__, __LINE__, fei::Logger::WARN, __func__).stream()
#define LOG_FATAL  fei::Logger(__FILE__, __LINE__, fei::Logger::FATAL, __func__).stream()



#endif