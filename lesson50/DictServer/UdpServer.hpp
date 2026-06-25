#pragma once
#include <iostream>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <functional>
#include "Logger.hpp"
using namespace LogModule;

// 参数就是获得的数据，返回值就是处理完数据的结果
using callback_t = std::function<std::string(const std::string &)>;

class UdpServer
{
public:
    // UdpEchoServer(const std::string &ip, uint16_t port)
    UdpServer(callback_t cb, uint16_t port)
        : _sockfd(-1),
          _port(port),
          _cb(cb)
    {
    }
    void Init()
    {
        // 1. 创建socket,只是系统概念
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0); // IPPROTO_UDP
        if (_sockfd < 0)
        {
            LOG(LogLevel::FATAL) << "create socket error";
            exit(1);
        }
        LOG(LogLevel::INFO) << "create socket fd success: " << _sockfd; // 3

        // 2. bind
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        // 如果server 显示的bind了一个具体IP地址，那么它一般就只能收到发给这个IP地址的报文
        // local.sin_addr.s_addr = inet_addr(_ip.c_str()); // 1. 字符串点分十进制IP—> 4字节 2. 4字节IP是网络序列的
        local.sin_addr.s_addr = INADDR_ANY; // 任意IP地址bind！

        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "bind socket error";
            exit(2);
        }
        LOG(LogLevel::INFO) << "bind socket fd success: " << _sockfd; // 3
    }
    void Start()
    {
        char inbuffer[1024];
        while (true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            // 读取网络数据
            ssize_t n = recvfrom(_sockfd, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (n < 0)
            {
                LOG(LogLevel::WARNING) << "recvfrom error";
                break;
            }
            inbuffer[n] = 0;
            uint16_t clientport = ntohs(peer.sin_port);
            std::string clientip = inet_ntoa(peer.sin_addr);

            // 网络序列数据
            LOG(LogLevel::INFO) << "get a message: " << inbuffer
                                << ", client addr: " << clientip << ":" << clientport;

            // 处理数据
            std::string result;
            if(_cb)
            {
                result = _cb(inbuffer);
            }

            // 发送网络数据
            int m = sendto(_sockfd, result.c_str(), result.size(), 0, (struct sockaddr *)&peer, len);
            (void)m;
        }
    }
    ~UdpServer() {}

private:
    int _sockfd;
    // std::string _ip; // ??
    uint16_t _port; // 端口号

    // 服务器未来收到数据，应该怎么处理？
    callback_t _cb;
};