#include"TcpEchoServer.hpp"
#include<iostream>
#include<memory>
#include<string>

void Usage(std::string procname)
{
    std::cout << "Usage: " << procname << " ServerPort" << std::endl;
}

// ./tcp_echo_server 8080
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        exit(1);
    }

    ENABLE_CONSOLE_LOG_STRATEGY();
    uint16_t port = std::stoi(argv[1]);

    std::unique_ptr<TcpEchoServer> tsvr = std::make_unique<TcpEchoServer>(port);
    tsvr->Init();
    tsvr->Start();

    return 0;
}