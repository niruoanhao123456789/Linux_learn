#include "Calculator.hpp"
#include "Protocol.hpp"
#include "Connection.hpp"
#include "Reactor.hpp"
#include "Listener.hpp"
#include <memory>

static const int gport = 8080;

int main()
{
    ENABLE_CONSOLE_LOG_STRATEGY();
    // 1. 定义计算器对象
    std::unique_ptr<Calculator> cal = std::make_unique<Calculator>();

    // 2. 定义协议对象
    std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>(
        [&cal](const Request& req) -> Response{
            return cal->Exec(req);
        }
    );

    // 3. listener
    std::shared_ptr<Connection> conn = std::make_shared<Listener>(gport,
        [&protocol](std::string& inbuffer, int* code)->std::string{
            return protocol->HandlerRequest(inbuffer, code);
        }
    );
    conn->SetEvents(EPOLLIN | EPOLLET);

    // 4. Reactor
    std::unique_ptr<Reactor> r = std::make_unique<Reactor>();

    // 5. listener -> Reactor
    r->AddConnection(conn);

    // 6. 开始事件派发
    r->Dispatcher();

    return 0;
}