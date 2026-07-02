#pragma once
#include<iostream>
#include<string>
#include<memory>
#include<unistd.h>
#include<signal.h>
#include<functional>
#include"Socket.hpp"

using handler_t = std::function<std::string(std::string&)>;

class TcpServer
{
public:
    TcpServer(uint16_t port,handler_t handler)
    :_port(port)
    ,_listensockfd(std::make_unique<TcpSocket>())
    ,_isrunning(false)
    ,_handlerstream(handler)
    {
        _listensockfd->BuildSocketMethod(_port); // 模板方法模式
    }

    void HanderIO(std::shared_ptr<Socket> sockfd,InetAddr clientaddr)
    {
        // 长链接服务
        std::string inbuffer;
        while(1)
        {
            int n = sockfd->Recv(&inbuffer);
            if(n<0)
            {
                LOG(LogLevel::WARNING)<<"recv error";
                break;
            }
            if(n == 0)
            {
                LOG(LogLevel::INFO)<<"client quit: "<<clientaddr.StringAddress()<<"sockfd: "<<sockfd->Socketfd();
                break;
            }

            LOG(LogLevel::DEBUG)<<inbuffer;

            // 1. 分析inbuffer保证报文的完整性
            // 2. a. 完整: 提取，处理 b.不完整：什么都不做
            // 3：这个工作谁做？协议来做！
            // 谁不做？？TcpServer
            std::string outbuffer;
            if(_handlerstream)
                outbuffer = _handlerstream(inbuffer); // 回调函数会调出去，也会再回来
            if(!outbuffer.empty())
                sockfd->Send(outbuffer);
        }

        sockfd->Close();
    }

    void Run()
    {
        _isrunning = true;
        signal(SIGCHLD,SIG_IGN);
        while(_isrunning)
        {
            InetAddr clientaddr;
            // 1. clientaddress 2. 得到socket
            auto sockfd = _listensockfd->Accepter(&clientaddr);
            if(!sockfd)
            {
                LOG(LogLevel::WARNING) << "accepter error";
                continue;
            }

            LOG(LogLevel::DEBUG) << "client addr: " << clientaddr.StringAddress() << " socket: " << sockfd->Socketfd();

            // 处理链接问题
            pid_t id = fork();
            if(id == 0)
            {
                _listensockfd->Close();
                // 子进程
                HanderIO(sockfd,clientaddr);
                exit(0);
            }
            // 父进程
            sockfd->Close();
        }
    }

private:
    int _port;
    std::unique_ptr<Socket> _listensockfd;
    bool _isrunning;
    handler_t _handlerstream;
};