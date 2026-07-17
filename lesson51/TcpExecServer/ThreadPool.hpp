#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include "Thread.hpp"
#include "Logger.hpp"
#include "Mutex.hpp"
#include "Cond.hpp"

static const int gnum = 5;

using namespace LogModule;

template <typename T>
class ThreadPool
{
private:
    bool IsTaskQueueEmpty()
    {
        return _queue.empty();
    }
    T PopHelper()
    {
        T t = _queue.front();
        _queue.pop();
        return t;
    }
    void ThreadRoutine()
    {
        char name[64];
        pthread_getname_np(pthread_self(), name, sizeof(name));
        while (true)
        {
            T task;
            {
                // 临界区
                LockGuard lockguard(&_lock);
                // 1. 没有任务 && 线程池不退出 -> 允许休眠
                while (IsTaskQueueEmpty() && _isrunning)
                {
                    _sleeper_cnt++;
                    LOG(LogLevel::DEBUG) << "没有任务, 线程休眠: |" << name << "|";
                    _cond.Wait(_lock);
                    LOG(LogLevel::DEBUG) << "有任务, 线程唤醒: |" << name << "|";
                    _sleeper_cnt--;
                }
                // 2. 没有任务 && 线程池退出 -- 线程结束
                if (IsTaskQueueEmpty() && !_isrunning)
                {
                    LOG(LogLevel::INFO) << "Thread: " << name << " quit";
                    break;
                }
                // 3. 有任务 && 线程池退出 , 不关心线程有没有退出
                // 3. 有任务 && 线程池没退出 , 不关心线程有没有退出
                // 目前: 一定是有任务的
                task = PopHelper();
            }
            task(); // 处理任务不应该在临界区内部处理，获取任务之后，任务已经从公有变成私有了
        }
    }

private:
    // 单例逻辑
    ThreadPool(int num = gnum) : _num(num), _isrunning(false), _sleeper_cnt(0)
    {
        for (int i = 0; i < _num; i++)
        {
            _threads.emplace_back([this]()
                                  { this->ThreadRoutine(); });
        }
    }

    ThreadPool(const ThreadPool<T> &) = delete;
    ThreadPool<T> &operator=(const ThreadPool<T> &) = delete;

public:
    // 获取单例，不是线程安全的！ -- 解决了
    // 双if判断，线程安全版的单例模式
    static ThreadPool<T> *GetInstance()
    {
        if (_instance == nullptr)
        {
            LockGuard lockguard(&_singleton_lock);
            if (_instance == nullptr)
            {
                LOG(LogLevel::DEBUG) << "首次使用，创建对象";
                _instance = new ThreadPool<T>(); // 只会做一次
                _instance->Start();
            }
        }
        return _instance;
    }
    void Start()
    {
        LockGuard lockguard(&_lock);
        if (_isrunning)
            return;
        _isrunning = true;
        for (auto &thread : _threads)
            thread.start();
    }
    void Enqueue(T task)
    {
        LockGuard lockguard(&_lock);
        if (!_isrunning) // 当线程池没有运行，禁止push任务
            return;
        _queue.push(task);
        if (_sleeper_cnt > 0)
            _cond.NotifyOne();

        LOG(LogLevel::DEBUG) << "2. Enqueue";
    }
    void Stop()
    {
        LockGuard lockguard(&_lock);
        if (_isrunning)
        {
            LOG(LogLevel::DEBUG) << "关闭线程池";
            _isrunning = false;
            if (_sleeper_cnt > 0)
                _cond.NotifyAll();
            // for (auto &thread : _threads)
            //     thread.stop();
        }
    }
    void Wait()
    {
        for (auto &thread : _threads)
            thread.join();
    }
    ~ThreadPool() {}

private:
    std::vector<Thread> _threads; // 所有线程
    int _num;
    bool _isrunning;
    // int _status; // running, stop, quit
    int _sleeper_cnt;

    std::queue<T> _queue;
    Mutex _lock;
    Cond _cond;

    // 单例模式
    static ThreadPool<T> *_instance;
    static Mutex _singleton_lock; // 保证单例线程安全的锁
};

template <typename T>
ThreadPool<T> *ThreadPool<T>::_instance = nullptr;

template <typename T>
Mutex ThreadPool<T>::_singleton_lock;