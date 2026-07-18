#pragma once
#include<iostream>
#include<string>
#include<memory>
#include<unistd.h>
#include<signal.h>
#include<functional>
#include"Socket.hpp"

using tcphandler_t = std::function<std::string(std::string &)>;

class TcpServer
{
public:
    TcpServer(uint16_t port)
    :_port(port)
    ,_listensockfd(std::make_unique<TcpSocket>())
    ,_isrunning(false)
    {
        _listensockfd->BuildSocketMethod(_port);
    }

    void HanderIO(std::shared_ptr<Socket> sockfd,InetAddr clientaddr)
    {
        std::string inbuffer;
        int n = sockfd->Recv(&inbuffer);
        if (n < 0)
        {
            LOG(LogLevel::WARNING) << "recv error";
        }
        if (n == 0)
        {
            LOG(LogLevel::INFO) << "client quit: " << clientaddr.StringAddress() << " sockfd: " << sockfd->Socketfd();
        }
        // 1. 分析inbuffer保证报文的完整性
        // 2. a. 完整: 提取，处理 b.不完整：什么都不做
        // 3：这个工作谁做？协议来做！
        // 谁不做？？TcpServer
        std::string outbuffer;
        if(_handlerstream)
            outbuffer = _handlerstream(inbuffer);

        if(!outbuffer.empty())
            sockfd->Send(outbuffer);
        sockfd->Close();
    }

    void Run(tcphandler_t handler)
    {
        _handlerstream = handler;
        _isrunning = true;
        signal(SIGCHLD,SIG_IGN);
        while(_isrunning)
        {
            InetAddr clientaddr;
            auto sockfd = _listensockfd->Accepter(&clientaddr);
            if(!sockfd)
            {
                LOG(LogLevel::WARNING)<<"accepter error";
                continue;
            }
            LOG(LogLevel::DEBUG)<<"clinet addr: "<<clientaddr.StringAddress()<<" sockfd: "<< sockfd->Socketfd();

            //处理链接问题
            if(fork()==0)
            {
                _listensockfd->Close();
                HanderIO(sockfd,clientaddr);
                exit(0);
            }

            sockfd->Close();
        }
    }

    ~TcpServer(){}

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensockfd;
    bool _isrunning;
    tcphandler_t _handlerstream;
};