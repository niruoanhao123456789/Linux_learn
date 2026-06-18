#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include"Mutex.hpp"
#include"Cond.hpp"
#include"Thread.hpp"
#include"Logger.hpp"
using namespace LogModule;

static const int gnum = 5;

template<typename T>
class ThreadPool
{
public:
    ThreadPool(int num = gnum)
    :_num(num)
    ,_isrunning(false)
    ,_sleeper_cnt(0)
    {
        for(int i=0;i< _num;i++)
        {
            _threads.emplace_back([this](){this->ThreadRoutine();});
        }
    }

    void Start()
    {
        LockGuard lockguard(&_mutex);
        if(_isrunning)
            return;
        _isrunning = true;
        for(auto& thread:_threads)
            thread.start();
    }

    void Push(const T& task)
    {
        LockGuard locjguard(&_mutex);
        // 当线程池没有运行，禁止push任务
        if(!_isrunning)
            return;
        _queue.push(task);
        if(_sleeper_cnt>0)
            _cond.NotifyOne();
    }

    void Stop()
    {
        LockGuard lockguard(&_mutex);
        if(_isrunning)
        {
            LOG(LogLevel::DEBUG)<<"关闭线程池";
            _isrunning = false;
            if(_sleeper_cnt>0)
                _cond.NotifyAll();
        }
    }

    void Wait()
    {
        for(auto& thread:_threads)
            thread.join();
    }

private:
    bool IsTaskQueueEmpty()
    {
        return _queue.empty();
    }

    T PopHelper()
    {
        T tmp = _queue.front();
        _queue.pop();
        return tmp;
    }

    void ThreadRoutine()
    {
        char name[64];
        pthread_getname_np(pthread_self(),name,sizeof(name));
        while(1)
        {
            T task;
            // 临界区
            LockGuard lockguard(&_mutex);
            // 1、没有任务与线程不退出的情况下允许休眠
            while(IsTaskQueueEmpty()&&_isrunning)
            {
                _sleeper_cnt++;
                LOG(LogLevel::DEBUG) << "没有任务线程休眠：|"<<name<<"|";
                _cond.Wait(_mutex);
                LOG(LogLevel::DEBUG) << "有任务，唤醒线程：|"<<name<<"|";
                _sleeper_cnt--;
            }
            // 2、没有任务与线程退出 -> 线程结束
            if(IsTaskQueueEmpty()&&!_isrunning)
            {
                LOG(LogLevel::INFO)<<"Thread: "<<name<<" quit";
                break;
            }
            // 3、 有任务 && 线程池退出 , 不关心线程有没有退出
            // 3、 有任务 && 线程池没退出 , 不关心线程有没有退出
            // 目前: 一定是有任务的
            task = PopHelper();
             // 处理任务不应该在临界区内部处理，获取任务之后，任务已经从公有变成私有了
            task();
        }
    }


    std::vector<Thread> _threads; // 所有线程
    int _num;
    bool _isrunning;
    // int _status; // running, stop, quit
    int _sleeper_cnt;

    std::queue<T> _queue;
    Mutex _mutex;
    Cond _cond;
};