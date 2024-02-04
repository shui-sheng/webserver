#ifndef _ATOMIC_H_
#define _ATOMIC_H_

#include<stdint.h>

namespace fei
{

template<class T>
class AtomicInt
{

public:
    AtomicInt()
        :value_(0)
    {
    }

    T get() const 
    {
        return __sync_val_compare_and_swap(const_cast<volatile T*>(&value_), 0, 0);
    }
    
    T getAndAdd(T x)
    {
        return __sync_fetch_and_add(&value_, x);
    }

    T addAndGet(T x)
    {
        return __sync_add_and_fetch(&value_, x);
    }

    T incrementAndGet()
    {
        return addAndGet(1);
    }

    void add(T x)
    {
        addAndGet(x);
    }

    void increment()
    {
        incrementAndGet();
    }

    void decrement()
    {
        addAndGet(-1);
    }

    T getAndSet(T x)
    {
        return __sync__lock_test_and_set(&value_, x);
    }

private:
    volatile T value_;
};

typedef AtomicInt<int32_t> AtomicInt32;

typedef AtomicInt<int64_t> AtomicInt64;

}

#endif