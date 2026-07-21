#include "ExecuteCommand.hpp"
#include "TcpServer.hpp"

#include <iostream>
#include <memory>
#include <string>

void Usage(std::string procname)
{
    std::cout << "Usage: " << procname << " ServerPort" << std::endl;
}


// ./tcp_echo_server 8080
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }

    ENABLE_CONSOLE_LOG_STRATEGY();
    uint16_t port = std::stoi(argv[1]);

    // 1. 创建业务层软件
    std::unique_ptr<ExecuteCommand> Exec = std::make_unique<ExecuteCommand>();

    // 2. 创建网路通信模块
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(port);

    // 3. 关联
    tsvr->Init([&Exec](std::string cmdstring)->std::string{
        return Exec->Exec(cmdstring);
    });
    tsvr->Start();

    return 0;
}