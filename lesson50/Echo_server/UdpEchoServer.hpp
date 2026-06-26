#pragma once
#include<iostream>
#include<string>
#include<strings.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"Logger.hpp"
using namespace LogModule;

class UdpEchoServer
{
public:
    UdpEchoServer(uint16_t port)
    :_sockfd(-1)
    ,_port(port)
    {}

    void Init()
    {
        // 1、创建socket,只是系统概念
        _sockfd = socket(AF_INET,SOCK_DGRAM,0); // IPPROTO_UDP
        if(_sockfd<0)
        {
            LOG(LogLevel::FATAL)<<"create socket error";
            exit(1);
        }
        LOG(LogLevel::INFO)<<"create socket fd success: "<<_sockfd;

        // 2、bind
        struct sockaddr_in local;
        bzero(&local,sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        // 如果server 显示的bind了一个具体IP地址，那么它一般就只能收到发给这个IP地址的报文
        // 1. 字符串点分十进制IP—> 4字节 2. 4字节IP是网络序列的
        // local.sin_addr.s_addr = inet_addr(_ip.c_str());
        local.sin_addr.s_addr = INADDR_ANY; // 任意IP地址bind

        int n = bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
        if(n<0)
        {
            LOG(LogLevel::FATAL)<<"bind socket error";
            exit(2);
        }
        LOG(LogLevel::INFO)<<"bind socket fd success: "<<_sockfd;
    }

    void Start()
    {
        char inbuffer[1024]={0};
        while(1)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            // 读取网路数据
            ssize_t n = recvfrom(_sockfd, inbuffer, sizeof(inbuffer)-1, 0, (struct sockaddr*)&peer, &len);
            if(n<0)
            {
                LOG(LogLevel::WARNING)<<"recvfrom error";
                break;
            }
            inbuffer[n] = 0;
            uint16_t clientport = ntohs(peer.sin_port);
            std::string client_ip = inet_ntoa(peer.sin_addr);

            // 网络序列数据
            LOG(LogLevel::INFO)<<"get a message: "<<inbuffer<<", client addr: "<<client_ip<<":"<<clientport;
            
            // 处理数据
            std::string echo_string = "server say: ";
            echo_string += inbuffer;

            // 发送网络数据
            sendto(_sockfd,echo_string.c_str(),echo_string.size(),0,(struct sockaddr*)&peer,len);
        }
    }

private:
    int _sockfd;
    uint16_t _port; // 端口号
};