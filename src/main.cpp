
// #include "../include/EventLoop.h"
// #include"../include/Channel.h"
// #include"../include/EventLoopThread.h"
// #include"../include/InetAddr.h"
// #include"../include/Server.h"
// #include"../include/log/Logger.h"
// #include<strings.h>
// #include <stdio.h>
// #include <sys/timerfd.h>

// #include <stdio.h>

// using namespace fei;
// void runInThread()
// {
//   printf("runInThread(): pid = %d, tid = %d\n",
//          getpid(), fei::CurrentThread::tid());
// }

// int main()
// {
//   printf("main(): pid = %d, tid = %d\n",
//          getpid(), fei::CurrentThread::tid());

//   // fei::EventLoopThread loopThread;
//   // fei::EventLoop* loop = loopThread.startLoop();
//   // loop->runInLoop(runInThread);
//   // sleep(1);
//   // loop->runAfter(2, runInThread);
//   // sleep(3);

//   //loop->quit();
  
//   //   EventLoop loop;
//   //   InetAddr addr(9981);
// 	// Server serv(&loop, addr);
// 	// serv.start();


// 	// loop.loop();
//   // printf("exit main().\n");


//     LOG_INFO<<"THISISAINFO";

// }

// #include "../include/log/AsyncLogging.h"
// #include "../include/log/Logger.h"
// #include "../include//time/Timestamp.h"

// #include <stdio.h>
// #include <unistd.h>

// using namespace fei;

// static const off_t kRollSize = 1*1024*1024;
// AsyncLogging* g_asyncLog = NULL;


// void test_AsyncLogging()
// {
//     const int n = 3*1024*1024;
//     for (int i = 0; i < n; ++i) {
//         LOG_INFO << "Hello, <<"<< " abc...xyz";
//     }
// }

// void asyncLog(const char* msg, size_t len)
// {
//     AsyncLogging* logging = g_asyncLog;
//     if (logging)
//     {
//         logging->append(msg, len);
//     }
// }

// int main(int argc, char* argv[])
// {
//     printf("pid = %d\n", getpid());

//     AsyncLogging log("bsname", 50, 100);


//     sleep(1);

//     g_asyncLog = &log;
//     Logger::setOutputFunc(asyncLog); // 为Logger设置输出回调, 重新配接输出位置
//     log.start();

//     test_AsyncLogging();

//     sleep(1);
//     log.stop();
//     return 0;
// }
#include"../include/net/EventLoop.h"
#include "../include/log/AsyncLogging.h"
#include "../include/log/Logger.h"
#include "../include//time/Timestamp.h"
#include"../include/net/Server.h"
#include"../include/net/Server.h"
#include<stdio.h>
#include<unistd.h>
#include <sys/timerfd.h>
#include<functional>

#include"../include/thread/ThreadPool.h"
using namespace fei;
AsyncLogging* g_log = NULL;

void asyncLog(const char* msg, size_t len)
{
    AsyncLogging* logging = g_log;
    if (logging)
    {
        logging->append(msg, len);
    }
}
void threadInit(EventLoop* loop)
{
    LOG_INFO<< "EventLoopThread " << loop << "is created";
}
int main()
{
    AsyncLogging log("/log/LogName", 100, 1000);
    g_log = &log;
    Logger::setOutputFunc(asyncLog);
    log.start();

    EventLoop baseloop;
    InetAddr localAddr(9981);
    
    Server server(&baseloop, localAddr, "server");
    server.setThreadInitCallback(threadInit);

    server.setThreadNum(10);
    server.start();
    
    baseloop.loop();





    //thread1.join();
}