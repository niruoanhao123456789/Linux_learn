#pragma once
#include<iostream>
#include<pthread.h>

class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&_lock,nullptr);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&_lock);
    }

    void Lock()
    {
        pthread_mutex_lock(&_lock);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&_lock);
    }

private:
    pthread_mutex_t _lock;
};

// 锁的开关
class LockGuard
{
public:
    LockGuard(Mutex* plock)
    :_plock(plock)
    {
        _plock->Lock();
    }

    ~LockGuard()
    {
        _plock->Unlock();
    }

private:
    Mutex* _plock;
};