#include "PollServer.hpp"

#include <memory>

const uint16_t gport = 8080;

int main()
{
    std::unique_ptr<PollServer> poll_svr = std::make_unique<PollServer>(gport);
    poll_svr->Dispatcher();

    return 0;
}