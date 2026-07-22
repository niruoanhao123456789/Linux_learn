#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include "Logger.hpp"
#include "Connection.hpp"
#include "Poller.hpp"

static const int gnum = 128;

class Reactor
{
private:
    bool IsConnectionExists(int sockfd)
    {
        return _connections.find(sockfd) != _connections.end();
    }

public:
    Reactor()
    :_epoller(std::make_unique<Poller>())
    {}

    // 新增Connection的接口
    void AddConnection(std::shared_ptr<Connection>& conn)
    {
        // 1. conn->sockfd && event 写到内核
        int sockfd = conn->Sockfd();
        uint32_t events = conn->Events();
        _epoller->AddEvents(sockfd,events);
        // 2. conn托管给_connections
        _connections[sockfd] = conn;
        // 3. conn回指Reactor
        conn->_r = this; // 指向当前对象

        LOG(LogLevel::INFO) << "insert "<< conn->Sockfd() << "into reactor!";
    }

    void EnableReadWrite(int sockfd, bool enableread, bool enablewrite)
    {
        if(!IsConnectionExists(sockfd))
            return;
        uint32_t events = ((enableread ? EPOLLIN : 0) | (enablewrite ? EPOLLOUT : 0) | EPOLLET);
        _connections[sockfd]->SetEvents(events);
        // 写到内核
        _epoller->ModEvents(sockfd,events);
    }

    void DelConnection(int sockfd)
    {
        if(!IsConnectionExists(sockfd))
            return;
        
        LOG(LogLevel::INFO) << "delete sockfd: " << sockfd;
        // 1. 从epoll中移除
        _epoller->DelEvents(sockfd);

        // 2. 关闭fd
        _connections[sockfd]->Close();

        // 3. 从_connections移除
        _connections.erase(sockfd);
    }

    void LooPOnce(int timeout)
    {
        int n = _epoller->WaitEvents(revs,gnum,timeout);
        for(int i = 0; i < n; i++)
        {
            // 哪一个fd
            int sockfd = revs[i].data.fd;
            // 哪些事件
            uint32_t revents = revs[i].events;

            if((revents & EPOLLERR) || (revents & EPOLLHUP))
                revents = (EPOLLIN | EPOLLOUT); // 把错误，转换成读写
            if((revents & EPOLLIN) && IsConnectionExists(sockfd))
                _connections[sockfd]->Recver();
            if((revents & EPOLLOUT) && IsConnectionExists(sockfd))
                _connections[sockfd]->Sender();
        }
    }

    void ConnectionKeepAlive()
    {

    }

    void Dispatcher()
    {
        int timeout = 1000;
        while(1)
        {
            DebugPrint();
            LooPOnce(timeout);
            ConnectionKeepAlive();
        }

    }

    void DebugPrint()
    {
        std::cout << "Reactor sockfd list: ";
        for (auto &conn : _connections)
        {
            std::cout << conn.second->Sockfd() << " ";
        }
        std::cout << std::endl;
    }

    ~Reactor() {}

private:
    // 1. epoller 模型
    std::unique_ptr<Poller> _epoller; // 我们从epoll这里拿到的之后，哪一个fd的那些事件就绪！
    // 2.组织所有的Connection
    std::unordered_map<int,std::shared_ptr<Connection>> _connections; // 服务器内所有的Connection
    // Heap<std::shared_ptr<Connection>> Heap;
    // 3. 已经就绪的事件清单
    struct epoll_event revs[gnum];
};