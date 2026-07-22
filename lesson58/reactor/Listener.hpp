#pragma once

#include <iostream>
#include <string>
#include <memory>
#include "Socket.hpp"
#include "Logger.hpp"
#include "Connection.hpp"
#include "IOHandler.hpp"

// 连接管理器 先描述
class Listener : public Connection
{
public:
    Listener(uint16_t port,OnMessage_t on_message)
    :_port(port)
    ,_on_message(on_message)
    ,_listensock(std::make_unique<TcpSocket>())
    {
        _listensock->BuildSocketMethod(_port);
        LOG(LogLevel::INFO) << "Listen sockfd create success: " << _listensock->Socketfd();
    }

    int  Sockfd() override { return _listensock->Socketfd(); }
    void Close () override { _listensock->Close(); }
    void Recver() override
    {
        LOG(LogLevel::INFO) << "Listener event ready, sockfd is : " << _listensock->Socketfd();
        while(1)
        {
            int errcode = 0;
            InetAddr clientaddr;
            int sockfd = _listensock->Accepter(&clientaddr, &errcode);
            if(sockfd >= 0)
            {
                // success
                LOG(LogLevel::INFO) << "get a new sockfd success: " << sockfd << " client addr: " << clientaddr.StringAddress();
                // 可以直接recv吗？？？recv(sockfd)?? 不能！！
                // a. sockfd 包装成为Connection -> IOHandler
                SetNonBlock(sockfd);  // 1. 设置非阻塞
                std::shared_ptr<Connection> conn = std::make_shared<IOHandler>(sockfd,_on_message); // 2. sockfd包装成为Connection
                conn->SetEvents(EPOLLIN | EPOLLET); // 3. 设置sockfd关心的事件
                conn->SetAddress(clientaddr);
                // b. sockfd上面的事件就绪，你怎么知道？？epoll->Reactor!!
                _r->AddConnection(conn);
            }
            else
            {
                // 不一定是真的失败，也可能是对方底层没有新链接了！！
                // 你怎么知道是出错了，还是底层没有连接了？？
                if (errcode == EAGAIN || errcode == EWOULDBLOCK)
                {
                    LOG(LogLevel::INFO) << "accepter Finish!";
                    break;
                }
                if (errcode == EINTR)
                {
                    LOG(LogLevel::INFO) << "accepter interupt!";
                    continue;
                }
                else
                {
                    LOG(LogLevel::ERROR) << "accepter error";
                    break;
                }
            }
        }
    }

    void Sender() override
    {
        // 忽略
    }
    void Excepter() override
    {
        // 忽略
    }

private:
    uint16_t _port;
    OnMessage_t _on_message;
    std::unique_ptr<Socket> _listensock;
};