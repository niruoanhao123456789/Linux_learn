#pragma once
#include<iostream>
#include<string>
#include<functional>
#include<pthread.h>
#include<unistd.h>
#include<sys/syscall.h>

using func_t = std::function<void()>;

enum class TSTATUS
{
    THREAD_NEW,
    THREAD_RUNNING,
    THREAD_STOP
};

// 这种写法会有个小bug
static int gnum = 1;
class Thread
{
public:
    Thread(func_t f)
    :_joinable(true)
    ,_status(TSTATUS::THREAD_NEW)
    ,_func(f)
    {
        _name = "thread-"+std::to_string(gnum++);
    }

    void start()
    {
        if(_status == TSTATUS::THREAD_RUNNING)
        {
            std::cout<<"thread is already running"<<std::endl;
            return;
        }
        pthread_create(&_tid,nullptr,routine,this);
        _status = TSTATUS::THREAD_RUNNING;
    }

    void stop()
    {
        if(_status == TSTATUS::THREAD_RUNNING)
        {
            pthread_cancel(_tid);
            _status = TSTATUS::THREAD_STOP;
        }
        else
        {
            std::cout<<"thread status is : THREAD_NEW or THREAD_STOP! stop error"<<std::endl;
        }
    }

    void join()
    {
        if(_joinable)
        {
            pthread_join(_tid,nullptr);
            printf("lwp : %d, name: %s, join success\n",_lwpid,_name.c_str());
        }
        else
        {
            printf("lwp : %d, name: %s, join failed, because thread is detach\n", _lwpid, _name.c_str());
        }
    }

    void detach()
    {
        if(_joinable && _status == TSTATUS::THREAD_RUNNING)
        {
            _joinable = false;
            pthread_detach(_tid);
        }
    }

    std::string Name()
    {
        return _name;
    }

private:
    void getprocessid()
    {
        _pid = getpid();
    }

    void getlwp()
    {
        _lwpid = syscall(SYS_gettid);
    }

    static void* routine(void* args)
    {
        Thread* ts = static_cast<Thread*>(args);
        ts->getprocessid();
        ts->getlwp();
        pthread_setname_np(pthread_self(),ts->_name.c_str());
        ts->_func();

        return nullptr;
    }

    pthread_t _tid;
    pid_t _pid;
    pid_t _lwpid;
    std::string _name;
    bool _joinable;
    TSTATUS _status;
    func_t _func;
};