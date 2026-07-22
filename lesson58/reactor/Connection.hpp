#pragma once

// 每一个fd，后续都对应一个connection连接
#include <iostream>
#include <string>
#include "Common.hpp"
#include "InetAddr.hpp"

class Reactor;

// 基类，先描述
class Connection
{
public:
    Connection()
    :_events(0)
    ,_r(nullptr)
    {
    }

    uint32_t Events() { return _events; }
    void SetEvents(uint32_t events) { _events = events; }
    void SetAddress(const InetAddr& addr) { _clientaddr = addr; }

    virtual int Sockfd() = 0;
    virtual void Recver() = 0;
    virtual void Sender() = 0;
    virtual void Excepter() = 0;
    virtual void Close() = 0;

    void Update() { _lasttimestamp = CurrentTimeStamp(); }

    ~Connection() {}

protected:
    InetAddr _clientaddr;    // client socket地址
    uint32_t _events;        // Connection 关心什么事件
    uint64_t _lasttimestamp; // 设置最近活跃时间戳

public:
    Reactor* _r; // 回指指针
};