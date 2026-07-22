#pragma once

#include <iostream>
#include <string>
#include <functional>
#include "Connection.hpp"
#include "Logger.hpp"

using namespace LogModule;

static const int gbuffersize = 1024;

using OnMessage_t = std::function<std::string (std::string&,int* code)>;

// 只负责读取和发送
class IOHandler : public Connection
{
public:
    IOHandler(int sockfd,OnMessage_t on_message)
    :_sockfd(sockfd)
    ,_on_message(on_message)
    {}

    int  Sockfd() override { return _sockfd; }
    void Close () override { close(_sockfd); }
    void Recver() override
    {
        Update();
        LOG(LogLevel::INFO) << "IOHandler event ready, sockfd is : " << _sockfd;
        // 由于ET的要求，必须循环式的把本次数据全部读取完毕！
        char buffer[gbuffersize] = {0};
        while(1)
        {
            ssize_t n = recv(_sockfd,buffer,gbuffersize-1,0);
            if(n > 0)
            {
                buffer[n] = 0;
                _inbuffer += buffer;
            }
            else if(n == 0)
            {
                LOG(LogLevel::INFO) << "client quit, address is: " << _clientaddr.Addr() << " sockfd: "<< _sockfd;
                Excepter();
                return; // 注意！
            }
            else
            {
                // 不一定是出错了！
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    break;
                }
                else if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    LOG(LogLevel::ERROR) << "recv error, address is: " << _clientaddr.StringAddress() << " sockfd: " << _sockfd;
                    Excepter();
                    return; // 注意！
                }
            }
        }

        // 尝试检查 -> 处理，(收到的数据，是否有至少一个完整的报文？？如果有，你要提取，然后交给上层进行反序列化处理！) 这些工作，不应该让
        // IOHandler来做，而应该结合协议来做！！！
        // 如果没有，什么都不做！！ 网络版本计算器！
        int code = 0;
        std::string result = _on_message(_inbuffer,&code);
        if(code == 0)
            _outbuffer += result; // result: string(), result:response
        else
        {
            Excepter();
            return;
        }

        // version 1
        // if(!_outbuffer.empty())
        //     Sender();
        // version 2
        if(!_outbuffer.empty())
            _r->EnableReadWrite(_sockfd, true, true); // 使能写事件关心！
    }

    void Sender() override
    {
        Update();
        while(1)
        {
            ssize_t n = send(_sockfd, _outbuffer.c_str(), _outbuffer.size(), 0);
            if(n>=0)
            {
                _outbuffer.erase(0,n);
                if(_outbuffer.empty())
                    break;
            }
            else
            {
                // 不一定是出错了！
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // 本轮数据发送完毕
                    break;
                }
                else if(errno == EINTR)
                {
                    continue;
                }
                else
                {
                    // 发送失败
                    LOG(LogLevel::ERROR) << "send error, address is: " << _clientaddr.StringAddress() << " sockfd: " << _sockfd;
                    Excepter();
                    return;
                }
            }
        }

        //  此时一定没有错误！
        // 1. 刚好发送完毕 2. 没发完
        if(_outbuffer.empty())
            _r->EnableReadWrite(_sockfd, true, false);
        else // 没发完？发送条件不满足了!, 对conn，修改sockfd关心的事件，epoll ->EPOLLOUT
            _r->EnableReadWrite(_sockfd, true, true);
    }

    void Excepter() override
    {
        LOG(LogLevel::ERROR) << "Excepter, address is: " << _clientaddr.StringAddress() << " sockfd: " << _sockfd;
        _r->DelConnection(_sockfd);
    }

    ~IOHandler() {}

private:
    int _sockfd;
    OnMessage_t _on_message;
    std::string _inbuffer;  //  接收缓冲区，char queue
    std::string _outbuffer; //  输出缓冲区
};