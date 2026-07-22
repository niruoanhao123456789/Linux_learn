#pragma once
#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/time.h>
#include "Logger.hpp"

using namespace LogModule;

enum
{
    SOCKET_ERR = 1,
    BIND_ERR,
    LISTEN_ERR,
    EPOLL_ERR
};

void SetNonBlock(int fd)
{
    int flag = fcntl(fd,F_GETFL);
    if(flag < 0)
    {
        LOG(LogLevel::ERROR) << "fcntl error set: "<< fd << " non block failed";
        return;
    }
    fcntl(fd,F_SETFL,flag | O_NONBLOCK);
}

uint64_t CurrentTimeStamp()
{
    return (uint64_t)time(nullptr);
}