#ifndef _EVENTLOOPTHREADPOOL_H_
#define _EVENTLOOPTHREADPOOL_H_
#include"EventLoop.h"
#include"EventLoopThread.h"
#include<vector>
#include<iostream>
#include<memory>
namespace fei
{
class EventLoopThreadPool
{
public:
 	typedef std::function<void(EventLoop*)> ThreadEventInitCallback;

	EventLoopThreadPool(EventLoop* baseLoop);
	void setThreadNum(int num) { numThreads_ = num; }
	void start(const ThreadEventInitCallback& cb);
	EventLoop* getNextLoop();



private:
	//typedef std::vector<std::unique_ptr<EventLoopThread>> threads_;

	EventLoop* baseloop_;
	std::vector<EventLoopThread*> threads_;
	std::vector<EventLoop*> loops_;
	bool started_;
	int numThreads_;
	int next_;
	
};


}

#endif