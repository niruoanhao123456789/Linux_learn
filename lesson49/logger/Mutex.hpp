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

    void Lock()
    {
        pthread_mutex_lock(&_lock);
    }

    pthread_mutex_t* Orgin()
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