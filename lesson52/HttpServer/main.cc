#include "HttpServer.hpp"

void Usage(std::string procname)
{
    std::cout << "Usage: " << procname << " ServerPort" << std::endl;
}

void Login(const HttpRequest &req, HttpResponse &resp)
{
    std::string args = req["args"];
    std::cout << "Login service: haha: " << args  << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }

    ENABLE_CONSOLE_LOG_STRATEGY();
    uint16_t port = std::stoi(argv[1]);

    std::unique_ptr<HttpServer> hsvr = std::make_unique<HttpServer>(port);
    hsvr->Register("/login", Login);
    hsvr->Run();

    return 0;
}