#include "EpollServer.hpp"

#include <memory>


int main()
{
    std::unique_ptr<EpollServer> epoll_svr = std::make_unique<EpollServer>();
    epoll_svr->Dispatcher();

    return 0;
}