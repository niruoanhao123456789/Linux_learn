#pragma once

#include<iostream>
#include<vector>
#include<unistd.h>
#include<memory>
#include<sys/socket.h>
#include<poll.h>
#include"Socket.hpp"
#include"Logger.hpp"

#define NUM 1024
const int gdefaultfd = -1;

using namespace LogModule;

class PollServer
{
public:
    PollServer(uint16_t port)
    :_port(port)
    ,_listensockfd(std::make_unique<TcpSocket>())
    {
        _listensockfd->BuildSocketMethod(_port);
        for(int i=0; i < NUM; i++)
        {
            _fdevent[i].fd = gdefaultfd;
            _fdevent[i].events = _fdevent[i].revents = 0;
        }

        // 将listen sockfd添加到poll
        _fdevent[0].fd = _listensockfd->Socketfd();
        _fdevent[0].events |= POLLIN;
    }

    // 1. 全都是用来处理新链接到来的
    // 链接管理器
    void Accepter()
    {
        InetAddr clientaddr;
        int fd = _listensockfd->Accepter(&clientaddr); // 不会阻塞
        // 你得到了一个新的连接，这个连接怎么处理？？
        // recv(fd)?? 等 + 拷贝， 不能！！
        // fd -> 托管给select-> 只有select具有"等"的能力!-> 如何托管？？ -> 只要把fd添加的辅助数组即可！
        if(fd >= 0)
        {
            int pos=0;
            for(pos=0;pos<NUM;pos++)
            {
                if(_fdevent[pos].fd == gdefaultfd)
                    break;
            }

            if(pos >= NUM)
            {
                // Poll这里要扩容
                LOG(LogLevel::WARNING)<< "server is full!";
                close(fd);
            }
            else
            {
                _fdevent[pos].fd = fd;
                _fdevent[pos].events = POLLIN;
                _fdevent[pos].revents = 0;
            }
        }
        else
        {
            LOG(LogLevel::ERROR) << "Accepter Error!";
        }
    }

    // IO处理器
    void IOHandler(int i)
    {
        // 普通的文件描述符, bug
        char buffer[1024];
        ssize_t n = recv(_fdevent[i].fd,buffer,sizeof(buffer)-1,0); // 不会被阻塞
        if(n > 0)
        {
            buffer[n] = 0;
            std::cout << "client say@ " << buffer <<std::endl;

            // 直接写？？还是交给select帮我关心写事件就绪 -- TODO
            std::string sendstring = "echo# ";
            sendstring += buffer;
            send(_fdevent[i].fd, sendstring.c_str(), sendstring.size(), 0);
        }
        else if(n == 0)
        {
            LOG(LogLevel::INFO) << "sockfd is closed: " << _fdevent[i].fd;
            close(_fdevent[i].fd);
            _fdevent[i].events = _fdevent[i].revents = 0;
            _fdevent[i].fd = gdefaultfd;
        }
        else
        {
            LOG(LogLevel::ERROR) << "recv error!" << _fdevent[i].fd;
            close(_fdevent[i].fd);
            _fdevent[i].events =_fdevent[i].revents = 0;
            _fdevent[i].fd = gdefaultfd;
        }
    }

    void EventHandler()
    {
        // 就绪事件的fd，不止一个了
        for(int i=0; i < NUM; i++)
        {
            if(_fdevent[i].fd == gdefaultfd)
                continue;
            // 合法的fd && 就绪
            if(_fdevent[i].revents & POLLIN)
            {
                if(_fdevent[i].fd == _listensockfd->Socketfd())
                {
                    // 这个sockfd是listensockfd
                    Accepter();
                }
                else
                {
                    IOHandler(i);
                }
            }
        }
    }

    void PrintFds()
    {
        std::cout << "poll Server fds list: ";
        for (int i = 0; i < NUM; i++)
        {
            if (_fdevent[i].fd == gdefaultfd)
                continue;
            std::cout << _fdevent[i].fd << " ";
        }
        std::cout << std::endl;
    }

    // 事件派发器 && 事件处理器
    void Dispatcher()
    {
        while(1)
        {
            // int timeout = -1;
            int timeout = 2000;
            // For Test
            PrintFds();
            // 1. listen
            // 2. normal fd
            int n = poll(_fdevent,NUM,timeout);
            switch (n)
            {
            case 0:
                LOG(LogLevel::DEBUG) << "time out...";
                break;
            case -1:
                LOG(LogLevel::DEBUG) << "poll error";
                break;
            default:
                LOG(LogLevel::DEBUG) << "事件就绪...: n:" << n; // 任意一个fd都可能事件就绪
                EventHandler();
                break;
            }
        }
    }

    ~PollServer() {}

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensockfd;

    struct pollfd _fdevent[NUM];
};