#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h> /* Definition of SYS_* constants */

using func_t = std::function<void()>;

enum class TSTATUS
{
    THREAD_NEW,
    THREAD_RUNNING,
    THREAD_STOP
};

// bug
static int gcnt = 1;

class Thread
{
private:
    void getprocessid()
    {
        _pid = getpid();
    }

    void getlwp()
    {
        _lwpid = syscall(SYS_gettid);
    }

    static void *routine(void *args)
    {
        Thread *ts = static_cast<Thread *>(args);
        ts->getprocessid();
        ts->getlwp();
        pthread_setname_np(pthread_self(), ts->Name().c_str());
        ts->_func();

        return nullptr;
    }

public:
    Thread(func_t f) : _joinable(true), _status(TSTATUS::THREAD_NEW),_func(f)
    {
        _name = "Worker-" + std::to_string(gcnt++);
    }

    void start()
    {
        if (_status == TSTATUS::THREAD_RUNNING)
        {
            std::cout << "thread is already running" << std::endl;
            return;
        }
        int n = pthread_create(&_tid, nullptr, routine, this);
        (void)n;
        _status = TSTATUS::THREAD_RUNNING;
    }

    void stop()
    {
        if (_status == TSTATUS::THREAD_RUNNING)
        {
            int n = pthread_cancel(_tid);
            (void)n;
            _status = TSTATUS::THREAD_STOP;
        }
        else
        {
            std::cout << "thread status is : THREAD_NEW or THREAD_STOP! stop error" << std::endl;
        }
    }

    void join()
    {
        if (_joinable)
        {
            int n = pthread_join(_tid, nullptr);
            (void)n;
            printf("lwp : %d, name: %s, join success\n", _lwpid, _name.c_str());
        }
        else{
            printf("lwp : %d, name: %s, join failed, because thread is detach\n", _lwpid, _name.c_str());
        }
    }

    void detach()
    {
        if (_joinable && _status == TSTATUS::THREAD_RUNNING)
        {
            _joinable = false;
            int n = pthread_detach(_tid);
            (void)n;
        }
    }

    std::string Name()
    {
        return _name;
    }

private:
    pthread_t _tid;
    pid_t _pid;
    pid_t _lwpid;
    std::string _name;
    bool _joinable;
    TSTATUS _status;
    func_t _func;
};