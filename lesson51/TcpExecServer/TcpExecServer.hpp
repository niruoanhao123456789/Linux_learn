#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Logger.hpp"
#include "InetAddr.hpp"
#include "ThreadPool.hpp"
using namespace LogModule;

using callback_t = std::function<std::string(std::string)>;

static const uint16_t gdefaultport = 8080;
static const int gbacklog = 32;

class TcpServer
{
private:
    // sockfd: 既可以支持读，又可以支持写, TCP socket也是全双工的.
    void Service(int sockfd,InetAddr client)
    {
        char name[128]={0};
        pthread_getname_np(pthread_self(),name,sizeof(name));
        // 长连接服务, 不适合用线程池
        // 比较适合短服务
        while(1)
        {
            char inbuffer[1024];
            // 1. 读取数据
            // "ls -a -l" -> "ls -" | "ls -a -" | ...
            // 面向字节流，可能会有粘包问题
            ssize_t n = read(sockfd,inbuffer,sizeof(inbuffer)-1); // 默认字符串
            if(n>0)
            {
                inbuffer[n]=0;
                LOG(LogLevel::INFO)<<name<<" : "<<client.StringAddress()<<" say# "<<inbuffer;
            }
            else if(n==0)
            {
                LOG(LogLevel::INFO)<<client.StringAddress()<<" close sockfd: "<<sockfd<<", me too.";
            }
            else
            {
                LOG(LogLevel::ERROR)<<"read sockfd error"<<sockfd;
                break;
            }

            // 2. 处理命名字符串, inbuffer->string->命令字符串 -> ls -a -l/ rm -rf xxx/ pwd / which...
            std::string result = _cb(inbuffer);

            // 3. 写回数据
            n = write(sockfd,result.c_str(),result.size());
            if(n<=0)
            {
                LOG(LogLevel::ERROR)<<"write sockfd error"<<sockfd;
                break;
            }
        }
        close(sockfd);
    }
public:
    TcpServer(uint16_t port = gdefaultport)
    :_port(port)
    ,_listensockfd(-1)
    {
    }

    void Init(callback_t cb)
    {
        _cb = cb;

        // 1. 创建socket
        _listensockfd = socket(AF_INET,SOCK_STREAM,0); // IPPROTO_TCP
        if(_listensockfd<0)
        {
            LOG(LogLevel::FATAL)<<"socket error: "<<_listensockfd;
            exit(2);
        }
        LOG(LogLevel::INFO)<<"socket success: "<<_listensockfd;

        // 2. bind : 设置服务器的socket信息
        struct sockaddr_in local;
        memset(&local,0,sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = htonl(INADDR_ANY);
        int n = bind(_listensockfd,(const sockaddr*)&local,sizeof(local));
        if(n<0)
        {
            LOG(LogLevel::FATAL)<<"bind error: "<<_listensockfd;
            exit(3);
        }
        LOG(LogLevel::INFO)<<"bind success: "<<_listensockfd;

        // 3. 将socket设置为监听状态
        n = listen(_listensockfd,gbacklog);
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "listen error: " << _listensockfd;
            exit(4);
        }
        LOG(LogLevel::INFO) << "listen success: " << _listensockfd;
    }

    class ThreadData
    {
    public:
        ThreadData(int sockfd,InetAddr addr,TcpServer* owner)
        :_sockfd(sockfd)
        ,_address(addr)
        ,_owner(owner)
        {}

        int _sockfd;
        InetAddr _address;
        TcpServer* _owner;
    };

    static void* threadrun(void* args)
    {
        pthread_detach(pthread_self()); // 线程分离
        ThreadData* td = static_cast<ThreadData*>(args);
        td->_owner->Service(td->_sockfd,td->_address);

        delete td;
        return nullptr;
    }

    void Start()
    {
        signal(SIGCHLD, SIG_IGN); // 最佳实践
        while (1)
        {
            struct sockaddr_in clientaddr;
            socklen_t len = sizeof(clientaddr);
            int sockfd = accept(_listensockfd, (struct sockaddr *)&clientaddr, &len);
            if (sockfd < 0)
            {
                LOG(LogLevel::WARNING) << "accept error";
                continue;
            }
            InetAddr clientaddress(clientaddr);
            LOG(LogLevel::INFO) << "get a new link: " << clientaddress.StringAddress() << " sockfd: " << sockfd;

            // // version3 -- 线程池 -- 目前代码样例里-- bug? 应用场景方面的限制
            // // ThreadPool<task_t>::GetInstance()->Enqueue([sockfd, clientaddress, this]()
            // //                                            { Service(sockfd, clientaddress); });

            // // Version2 -- 多线程版本
            // // 多线程这里，不能关闭任何自己不用的sockfd
            // // pthread_t tid;
            // // ThreadData *td = new ThreadData(sockfd, clientaddress, this);
            // // pthread_create(&tid, nullptr, threadrun, (void *)td);

            // // 处理连接，进行IO通信
            // // Version0 -- 不会被使用
            // // Service(sockfd, clientaddress);

            // // Version1 -- 多进程
            pid_t id = fork();
            if (id < 0)
            {
                LOG(LogLevel::ERROR) << "fork error";
                close(sockfd);
            }
            else if (id == 0)
            {
                // 关闭自己并不需要的sockfd
                close(_listensockfd);
                // 子进程,拷贝父进程的文件描述符表，从而和父进程看到同一批文件
                if (fork() > 0)
                    exit(0);
                // 孙子进程
                Service(sockfd, clientaddress);
                exit(0);
            }

            close(sockfd);
            // 父进程
            waitpid(id, nullptr, 0);
        }
    }

private:
    uint16_t _port;
    int _listensockfd;
    callback_t _cb;
};