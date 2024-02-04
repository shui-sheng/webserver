#include"../../include/log/Logger.h"
#include"../../include/thread/Thread.h"
using namespace fei;

void defaultOutput(const char* msg, size_t len)
{
    size_t n = fwrite(msg, 1, len, stdout);
    //FIXME check n
    (void)n;
}
void defalutFlush()
{
    ::fflush(stdout);
}

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush  = defalutFlush;


const char* LogLevelName[static_cast<int>(Logger::LogLevel::NUM_LOG_LEVELS)]=
{
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "INFO",

};




Logger::Logger(const char* file, int line, LogLevel level,const char* func)
    :impl_(file, line, level, func)
{
    //printf("Logger:%s,%d,%s\n", file, line, func);
}
Logger::~Logger()
{
    //printf("this is ~Logger():");
    //正文后置构造
    impl_.finish();
    const LogStream::Buffer& buf(stream().buffer());
    g_output(buf.data(), buf.length());
    if(impl_.level_ == LogLevel::FATAL)
    {
        fwrite(buf.data(), 1, buf.length(), stderr);

        abort();
    }
    // output(impl_.stream().data(), impl_.stream())
    // printf("%s", impl_.stream().data());
    //LogStream::Buffer& buf()
}

Logger::Impl::Impl(const char* file, int line, LogLevel level,const char* func)
    :file_(file), line_(line), level_(level), func_(func)
{
    char buf[64];
    Timestamp tm(Timestamp::now());
    time_t seconds = tm.microSecondsSinceEpoch()/Timestamp::kMicroSecondsPerSecond;
    struct tm *date = gmtime(&seconds);
    
    //buf前置长度 = 20+6+5 = 31
    snprintf(buf,sizeof(buf),"%4d%02d%02d %02d:%02d:%02d.%06ld %s from thread %d "
                            , date->tm_year+1900, date->tm_mon+1, date->tm_mday
                            , date->tm_hour+8, date->tm_min, date->tm_sec
                            , tm.microSecondsSinceEpoch()%Timestamp::kMicroSecondsPerSecond
                            , LogLevelName[static_cast<int>(level)]
                            , CurrentThread::tid());
    //正文前置构造完毕
    stream_<<buf;

}
void Logger::Impl::finish()
{
    stream_ <<" - " << file_ << ":" << line_ ;
    stream_ <<  '\n';

}
void Logger::setOutputFunc(OutputFunc f)
{
    g_output = f;
}

