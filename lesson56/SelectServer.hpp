#pragma once

#include<iostream>
#include<unistd.h>
#include<memory>
#include<sys/socket.h>
#include<sys/select.h>
#include"Socket.hpp"
#include"Logger.hpp"

#define NUM (int)(sizeof(fd_set) * 8)
const int gdefaultfd = -1;

using namespace LogModule;

class SelectServer
{
public:
    SelectServer(uint16_t port)
    :_port(port)
    ,_listensockfd(std::make_unique<TcpSocket>())
    {
        _listensockfd->BuildSocketMethod(_port);
        for(int i=0;i<NUM;i++)
            array_fds[i] = gdefaultfd;
        
        array_fds[0] = _listensockfd->Socketfd(); // 首先把listensockfd放到辅助数组里
    }

    // 1. 全部都是用来处理新链接到来的
    // 连接管理器
    void Accepter()
    {
        InetAddr clientaddr;
        int fd = _listensockfd->Accepter(&clientaddr); // 会不会阻塞？不会！
        LOG(LogLevel::INFO) << "get a new link: " << fd;
        //你得到了一个新的连接，这个连接怎么处理？？
        // recv(fd)?? 等 + 拷贝， 不能！
        // fd -> 托管给select-> 只有select具有"等"的能力!-> 如何托管？？ -> 只要把fd添加的辅助数组即可！
        if(fd >= 0)
        {
            int pos = 0;
            while(pos<NUM)
            {
                if(array_fds[pos] == gdefaultfd)
                    break;
                pos++;
            }
            if(pos >= NUM)
            {
                LOG(LogLevel::WARNING) << "server is full!";
                close(fd);
            }
            else
            {
                array_fds[pos] = fd;
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
        ssize_t n = recv(array_fds[i],buffer,sizeof(buffer)-1,0); // 也不会被阻塞
        if(n > 0)
        {
            buffer[n] = 0;
            std::cout<<"client say@ "<< buffer << std::endl;

            // 直接写？？还是交给select帮我关心写事件就绪 -- TODO
            std::string sendstr = "echo# ";
            sendstr += buffer;
            send(array_fds[i],sendstr.c_str(),sendstr.size(),0);
        }
        else if(n == 0)
        {
            LOG(LogLevel::INFO) << "sockfd is closed: " << array_fds[i];
            close(array_fds[i]);       // 关闭fd
            array_fds[i] = gdefaultfd; // 取消select关心
        }
        else
        {
            LOG(LogLevel::ERROR) << "recv error!" <<array_fds[i];
            close(array_fds[i]);
            array_fds[i] = gdefaultfd;
        }
    }

    void EventHandler(fd_set& rfds)
    {
        // 就绪事件的fd，不止一个
        for(int i=0;i< NUM;i++)
        {
            if(array_fds[i] == gdefaultfd)
                continue;
            // 合法的fd与就绪
            if(FD_ISSET(array_fds[i],&rfds))
            {
                if(array_fds[i] == _listensockfd->Socketfd())
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
        std::cout << "Select Server fds list: ";
        for (int i = 0; i < NUM; i++)
        {
            if (array_fds[i] == gdefaultfd)
                continue;
            std::cout << array_fds[i] << " ";
        }
        std::cout << std::endl;
    }

    // 事件派发器 与 事件处理器
    void Dispatcher()
    {
        fd_set rfds; // read fd set
        while(1)
        {
            // rfds参数重置
            FD_ZERO(&rfds);
            int max_fd = gdefaultfd;
            for(int i=0;i<NUM;i++)
            {
                if(array_fds[i] == gdefaultfd)
                    continue;
                FD_SET(array_fds[i],&rfds);
                if(max_fd < array_fds[i])
                {
                    max_fd = array_fds[i];
                }
            }

            // For Test
            PrintFds();
            // 1、listen
            // 2、normal fd
            int n = select(max_fd + 1,&rfds,nullptr,nullptr,nullptr);
            switch(n)
            {
            case 0:
                LOG(LogLevel::DEBUG) << "time out...";
                break;
            case -1:
                LOG(LogLevel::DEBUG) << "select error";
                break;
            default:
                LOG(LogLevel::DEBUG) << "事件就绪...: n:" << n; // 任意一个fd都可能事件就绪
                EventHandler(rfds);
                break;
            }
        }
    }

    ~SelectServer() {}

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensockfd;

    // select 服务器，借助一个辅助数组
    int array_fds[NUM];
};