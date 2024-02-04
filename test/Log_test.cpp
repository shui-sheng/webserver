#include"../include/log/AsyncLogging.h"
#include"../include/log/Logger.h"
using namespace fei;
AsyncLogging* g_async = NULL;
void asyncoutput(const char* msg, size_t len)
{
    g_async->append(msg, len);
}
void printlog()
{
    while(1)
    {
        LOG_INFO<<"this is a test log from printlog";
        sleep(1);
    }
}

int main()
{
    AsyncLogging asynclog("basename", 100, 100);
    g_async = &asynclog;
    fei::Logger::setOutputFunc(asyncoutput);
    asynclog.start();
    printf("maincurrent=%d\n",CurrentThread::tid());
    Thread t2(printlog);
    t2.start();
    for(int i =0; i < 10; i++)
        LOG_INFO<<"THIS IS A INFO000";
    //LOG_FATAL<<"log fatal!!!!!";
    sleep(10);
}