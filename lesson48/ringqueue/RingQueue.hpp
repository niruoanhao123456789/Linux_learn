#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<pthread.h>
#include<semaphore.h>

static const int gdefaultcap = 5;

template<typename T>
class RingQueue
{
public:
    RingQueue(int cap = gdefaultcap)
    :_ringqueue(cap)
    ,_cap(cap)
    ,_p_step(0)
    ,_c_step(0)
    {
        sem_init(&_space_sem,0,cap);
        sem_init(&_data_sem,0,0);

        pthread_mutex_init(&_c_mutex,nullptr);
        pthread_mutex_init(&_p_mutex,nullptr);
    }

    void Pop(T* out)
    {
        // 消费者
        sem_wait(&_data_sem);
        pthread_mutex_lock(&_c_mutex);

        *out = _ringqueue[_c_step++];
        _c_step %= _cap;

        pthread_mutex_unlock(&_c_mutex);
        sem_post(&_space_sem);
    }

    void Push(T in)
    {
        // 生产者
        sem_wait(&_space_sem);
        pthread_mutex_lock(&_p_mutex);

        _ringqueue[_p_step++] = in;
        _p_step %= _cap;

        pthread_mutex_unlock(&_p_mutex);
        sem_post(&_data_sem);
    }

    ~RingQueue()
    {
        sem_destroy(&_space_sem);
        sem_destroy(&_data_sem);
        pthread_mutex_destroy(&_c_mutex);
        pthread_mutex_destroy(&_p_mutex);
    }

private:
    std::vector<T> _ringqueue;
    int _cap; // 容量

    int _p_step;
    int _c_step;

    sem_t _space_sem;
    sem_t _data_sem;

    pthread_mutex_t _c_mutex;
    pthread_mutex_t _p_mutex;
};