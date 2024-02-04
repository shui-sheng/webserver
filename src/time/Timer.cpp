#include"../../include/time/Timestamp.h"
#include"../../include/time/Timer.h"

using namespace fei;

void Timer::restart(Timestamp now)
{
    if(repeat_)
        expiration_ = addTime(now, interval_);
    else 
        expiration_ = Timestamp::invalid();
}