#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/epoll.h>
#include "Common.hpp"
#include "Logger.hpp"

static const int gsize = 128;

using namespace LogModule;

class Poller
{
private:
    int EpollCtlHelper(int sockfd,uint32_t events,int oper)
    {
        if(oper == EPOLL_CTL_DEL)
            return epoll_ctl(_epfd,oper,sockfd,nullptr);
    
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = sockfd;
        return epoll_ctl(_epfd,oper,sockfd,&ev);
    }

public:
    Poller()
    {
        _epfd = epoll_create(gsize);
        if(_epfd < 0)
        {
            LOG(LogLevel::FATAL) << "epoll_create error!";
            exit(EPOLL_ERR);
        }
        LOG(LogLevel::INFO) << "create epollfd success: " << _epfd;
    }

    void DelEvents(int sockfd)
    {
        EpollCtlHelper(sockfd,0,EPOLL_CTL_DEL);
    }

    void AddEvents(int sockfd,uint32_t events)
    {
        int n = EpollCtlHelper(sockfd,events,EPOLL_CTL_ADD);
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "AddEvents error!";
        }
        
    }

    void ModEvents(int sockfd, uint32_t events)
    {
        int n = EpollCtlHelper(sockfd, events, EPOLL_CTL_MOD);
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "ModEvents error!";
        }
    }

    int WaitEvents(struct epoll_event revs[], int num, int timeout)
    {
        int n = epoll_wait(_epfd, revs, num, timeout);
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "epoll_wait error!";
        }
        else if (n == 0)
        {
            LOG(LogLevel::INFO) << "epoll_wait time!";
        }
        return n;
    }

    ~Poller() {}

private:
    int _epfd;
};