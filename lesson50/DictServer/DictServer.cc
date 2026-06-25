#include <memory>

#include "Dictionary.hpp"
#include "UdpServer.hpp"

void Usage(const std::string &name)
{
    std::cerr << "Usage: " << name << " port" << std::endl;
}

// ./UdpEchoServer 8080
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }

    ENABLE_CONSOLE_LOG_STRATEGY();

    // std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[1]);

    // 1. 先要有一个在线的字典
    std::unique_ptr<Dictionary> dict = std::make_unique<Dictionary>();

    // 2. 需要一台网络服务器
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>([&dict](const std::string &word)->std::string{
        return dict->Translate(word);
    }, server_port);

    // 3. 初始化并启动
    usvr->Init();
    usvr->Start();

    return 0;
}