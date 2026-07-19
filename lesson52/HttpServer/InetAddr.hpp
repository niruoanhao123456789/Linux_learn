#pragma once

// 网络和本地socket转换的类

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CONV(addr) ((struct sockaddr *)(addr))

class InetAddr
{
public:
    InetAddr()
    {
    }
    // n to h
    InetAddr(struct sockaddr_in &addr) : _net_addr(addr)
    {
        _port = ntohs(_net_addr.sin_port);
        _ip = inet_ntoa(_net_addr.sin_addr);
    }
    InetAddr(uint16_t port, std::string ip = "0.0.0.0")
        : _port(port), _ip(ip)
    {
        _net_addr.sin_family = AF_INET;
        _net_addr.sin_port = htons(_port);
        _net_addr.sin_addr.s_addr = inet_addr(_ip.c_str()); // 等价 INADDR_ANY
    }
    uint16_t Port() { return _port; }
    std::string Ip() { return _ip; }
    struct sockaddr *Addr()
    {
        return CONV(&_net_addr);
    }
    bool operator==(const InetAddr &addr)
    {
        return (_ip == addr._ip) && (_port == addr._port); // ?
    }
    void operator=(const struct sockaddr_in &addr)
    {
        _net_addr = addr;
        _port = ntohs(_net_addr.sin_port);
        _ip = inet_ntoa(_net_addr.sin_addr);
    }
    socklen_t AddrLen()
    {
        return sizeof(_net_addr);
    }
    std::string StringAddress()
    {
        return "[" + _ip + ":" + std::to_string(_port) + "]";
    }
    ~InetAddr()
    {
    }

private:
    // 本地地址
    uint16_t _port;
    std::string _ip;
    // 网络地址
    struct sockaddr_in _net_addr;
};