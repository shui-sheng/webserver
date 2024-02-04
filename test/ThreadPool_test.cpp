#include"../include/thread/ThreadPool.h"
#include"../include/net/EventLoopThreadPool.h"


using namespace fei;


// void print()
// {
//     printf("currentid:%d\n", CurrentThread::tid());
//     while(1)
//     {
//         std::cout<<1;
//     }
// }
// void test(int maxsize)
// {
//     ThreadPool pool("threadPool1");
//     pool.setMaxQueueSize(10);
//     pool.start(3);

//     pool.run(print);
//     pool.run(print);
//     pool.stop();

// }
// int main()
// {
//     test(1);


// }
void init(EventLoop* loop)
{
    printf("current tid = %d, \n", CurrentThread::tid());
}
int main()
{
    EventLoop baseloop;
    EventLoopThreadPool pool(&baseloop);
    pool.setThreadNum(5);
    pool.start(init);







}