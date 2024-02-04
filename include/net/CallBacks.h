#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_


#include<functional>

namespace fei
{
    class Connection;
    typedef std::function<void()> ReadCallback;
    typedef std::function<void()> WriteCallback;
    typedef std::function<void()> CloseCallback;
    typedef std::function<void()> ErrorCallback;
    typedef std::function<void()> TimerCallback;
}




#endif