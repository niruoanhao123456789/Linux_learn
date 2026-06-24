#include "UdpServer.hpp"
#include "Route.hpp"
#include "ThreadPool.hpp"
#include <memory>
using task_t = std::function<void()>;

void Usage(const std::string &name)
{
    std::cerr << "Usage: " << name << " port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }

    ENABLE_CONSOLE_LOG_STRATEGY();

    uint16_t server_port = std::stoi(argv[1]);
    std::unique_ptr<Route> route = std::make_unique<Route>();
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(
        [&route](std::string message, InetAddr who, int sockfd)
        {
            LOG(LogLevel::INFO) << "1. get a message: " << message
                                << ", client addr: " << who.Ip() << ":" << who.Port();

            auto task = [&message, &who, &sockfd, &route]() -> void
            {
                route->RouteMessage(message, who, sockfd);
            };
            ThreadPool<task_t>::GetInstance()->Push(task);
        }
        ,server_port);

    usvr->Init();
    usvr->Start();

    return 0;
}