#pragma once
#include<iostream>
#include<string>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<memory>
#include"InetAddr.hpp"
#include"Logger.hpp"
using namespace LogModule;

static const int gbacklog = 16;

enum
{
    SOCKET_ERR = 1,
    BIND_ERR,
    LISTEN_ERR
};

// 基类
// 把socket创建过层，模版化，方法化 -- 模版方法模式
class Socket
{
public:
    virtual ~Socket(){}
    virtual void CreateSocketOrDie() = 0;
    virtual void BindSocketOrDie(uint16_t port) = 0;
    virtual void ListenSocketOrDie() = 0;
    virtual std::shared_ptr<Socket> Accepter(InetAddr* client_addr) = 0;
    virtual void ConnectOrDie(const std::string& server_ip,uint16_t server_port) = 0;
    virtual int Socketfd() = 0;
    virtual void Close() = 0;
    virtual int Recv(std::string* outstr) = 0;
    virtual int Send(const std::string& outstr) = 0;

    void BuildSocketMethod(uint16_t port)
    {
        CreateSocketOrDie();
        BindSocketOrDie(port);
        ListenSocketOrDie();
    }

    void BuildClientSocketMethod(const std::string& server_ip,uint16_t server_port)
    {
        CreateSocketOrDie();
        ConnectOrDie(server_ip,server_port);
    }
};

class TcpSocket : public Socket
{
public:
    TcpSocket()
    :_sockfd(-1)
    {}

    TcpSocket(int sockfd)
    :_sockfd(sockfd)
    {}

    void CreateSocketOrDie() override
    {
        _sockfd = socket(AF_INET,SOCK_STREAM,0);
        if(_sockfd<0)
        {
            LOG(LogLevel::FATAL)<<"create socket error";
            exit(SOCKET_ERR);
        }
        LOG(LogLevel::INFO)<<"create socket success";
    }

    void BindSocketOrDie(uint16_t port) override
    {
        InetAddr local(port);
        int n = bind(_sockfd,local.Addr(),local.AddrLen());
        if(n<0)
        {
            LOG(LogLevel::FATAL)<<"bind socket error";
            exit(BIND_ERR);
        }
        LOG(LogLevel::INFO)<<"bind socket success";
    }

    void ListenSocketOrDie() override
    {
        int n = listen(_sockfd,gbacklog);
        if(n<0)
        {
            LOG(LogLevel::FATAL)<<"listen socket error";
            exit(LISTEN_ERR);
        }
        LOG(LogLevel::INFO)<<"listen socket success";
    }

    std::shared_ptr<Socket> Accepter(InetAddr* clientaddr) override
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int sockfd = accept(_sockfd,CONV(&peer),&len);
        if(sockfd<0)
        {
            return nullptr;
        }
        *clientaddr = peer;
        return std::make_unique<TcpSocket>(sockfd);
    }

    int Socketfd() override
    {
        return _sockfd;
    }

    void Close() override
    {
        if(_sockfd>=0)
        {
            close(_sockfd);
            _sockfd = -1;
        }
    }

    int Recv(std::string* outstr) override
    {
        char buffer[1024] = {0};
        ssize_t n = recv(_sockfd,buffer,sizeof(buffer)-1,0);
        if (n > 0)
        {
            buffer[n] = 0;
            *outstr += buffer; // +=的本质是拼接，入队列，outstr当做一个字节流队列！
            return n;
        }
        else if (n == 0)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    int Send(const std::string& outstr) override
    {
        return send(_sockfd,outstr.c_str(),outstr.size(),0);
    }

    void ConnectOrDie(const std::string& server_ip,uint16_t server_port) override
    {
        InetAddr server_addr(server_port,server_ip);
        int n = connect(_sockfd,server_addr.Addr(),server_addr.AddrLen());
        if(n!=0)
        {
            LOG(LogLevel::FATAL) << "connect " << server_addr.StringAddress() << " failed";
            return;
        }
        LOG(LogLevel::INFO) << "connect " << server_addr.StringAddress() << " success";
    }

private:
    int _sockfd;
};