#ifndef _TIMERID_H_
#define _TIMERID_H_

namespace fei
{
class Timer;
class TimerId
{
public:
    explicit TimerId(Timer* timer)
        :value_(timer)
    {}
private:
    Timer* value_;
};
}

#endif