#pragma once
#include <iostream>
#include <queue>
#include "Mutex.hpp"
#include "Cond.hpp"

static const int gdefaultcapacity = 5;

template <class T>
class BlockQueue
{
public:
    BlockQueue(size_t capacity = gdefaultcapacity)
        : _productor_sleeper_cnt(0), _consumer_sleeper_cnt(0), _capacity(capacity), _low_water(capacity / 3), _high_water(capacity / 3 * 2)
    {
    }

    // 生产
    void Push(T in)
    {
        LockGuard lockguard(&_mutex);
        while (_queue.size() == _capacity)
        {
            _productor_sleeper_cnt++;
            _cond_consumer.Wait(_mutex);
            _productor_sleeper_cnt--;
        }
        _queue.push(in);

        _cond_consumer.NotifyOne();
    }

    void Pop(T *out)
    {
        // 消费
        {
            LockGuard lockguard(&_mutex); // 加锁
            while (_queue.empty())
            {
                _consumer_sleeper_cnt++;
                _cond_consumer.Wait(_mutex);
                _consumer_sleeper_cnt--;
            }

            *out = _queue.front();
            _queue.pop();

            // if (_queue.size() < _low_water && _productor_sleeper_cnt > 0)
            //     pthread_cond_signal(&_cond_productor); // ??
            _cond_productor.NotifyOne();
        }
    }

private:
    int _productor_sleeper_cnt;
    int _consumer_sleeper_cnt;

    size_t _capacity;
    size_t _low_water; // 有效数据太少了
    size_t _high_water;

    std::queue<T> _queue; // 临界资源
    Mutex _mutex;
    Cond _cond_productor; // full
    Cond _cond_consumer;  // empty
};