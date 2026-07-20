#pragma once
#include <iostream>
#include <pthread.h>

class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&_lock, nullptr);
    }

    void Lock()
    {
        pthread_mutex_lock(&_lock);
    }

    pthread_mutex_t *Orgin()
    {
        return &_lock;
    }

    void Unlock()
    {
        pthread_mutex_unlock(&_lock);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&_lock);
    }
    
private:
    pthread_mutex_t _lock;
};

// 锁的开关
class LockGuard
{
public:
    LockGuard(Mutex *lockp): _lockp(lockp)
    {
        _lockp->Lock();
    }

    ~LockGuard()
    {
        _lockp->Unlock();
    }

private:
    Mutex *_lockp;
};
