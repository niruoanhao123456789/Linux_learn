#pragma once
#include<iostream>
#include<vector>
#include<string>
#include"sem.hpp"
#include"Mutex.hpp"

static const int gdefaultcap = 5;

template <typename T>
class RingQueue
{
public:
    RingQueue(int cap = gdefaultcap)
        : _ringqueue(cap), _cap(cap), _p_step(0), _c_step(0),
          _space_sem(cap), _data_sem(0)
    {
    }
    ~RingQueue()
    {
    }
    void Pop(T *out)
    {
        // 消费者
        _data_sem.P();

        LockGuard lockguard(&_c_mutex);
        *out = _ringqueue[_c_step++];
        _c_step %= _cap;

        _space_sem.V();
    }
    void Push(T in)
    {
        // 生产者
        _space_sem.P();

        LockGuard lockguard(&_p_mutex);
        _ringqueue[_p_step] = in;
        _p_step++;
        _p_step %= _cap;
        
        _data_sem.V();
    }

private:
    std::vector<T> _ringqueue;
    int _cap; // 容量

    int _p_step;
    int _c_step;

    Semaphore _space_sem;
    Semaphore _data_sem;

    Mutex _c_mutex;
    Mutex _p_mutex;
};