#pragma once
#include<iostream>
#include<string>
#include<strings.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<functional>
#include"InetAddr.hpp"
#include"Logger.hpp"
using namespace LogModule;

// 参数就是获得的数据，返回值就是处理完数据的结果
using callback_t = std::function<void(std::string message,InetAddr who,int sockfd)>;

class UdpServer
{
public:
    UdpServer(callback_t cb,uint16_t port)
    :_sockfd(-1)
    ,_port(port)
    ,_cb(cb)
    {}

    void Init()
    {
        // 1、创建socket
        _sockfd = socket(AF_INET,SOCK_DGRAM,0);
        if(_sockfd<0)
        {
            LOG(LogLevel::FATAL)<<"create socket error";
            exit(1);
        }
        LOG(LogLevel::INFO)<<"create socket success";

        // 2、bind
        InetAddr local(_port);
        int n = bind(_sockfd,local.Addr(),local.AddrLen());
        if(n<0)
        {
            LOG(LogLevel::FATAL)<<"bind socket error";
            exit(2);
        }
        LOG(LogLevel::INFO)<<"bind socket fd sucsess: "<<_sockfd;
    }

    void Start()
    {
        char inbuffer[1024]={0};
        while(1)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            // 读取数据
            ssize_t n = recvfrom(_sockfd,inbuffer,sizeof(inbuffer)-1,0,(struct sockaddr*)&peer,&len);
            if (n < 0)
            {
                LOG(LogLevel::WARNING) << "recvfrom error";
                break;
            }
            inbuffer[n]=0;
            InetAddr clientaddr(peer);

            // 处理数据
            if(_cb)
            {
                _cb(inbuffer,clientaddr,_sockfd);
            }
        }
    }

    ~UdpServer()
    {
        if(_sockfd>=0)
        {
            close(_sockfd);
            _sockfd = -1;
        }
    }

private:
    int _sockfd;
    uint16_t _port;
    callback_t _cb;
};