#include "HttpServer.hpp"

void Usage(std::string procname)
{
    std::cout << "Usage: " << procname << " ServerPort" << std::endl;
}

// SessionManager sm;


void Login(const HttpRequest &req, HttpResponse &resp)
{
    std::string args = req["args"];
    std::cout << "-----> Login service, args: " << args << std::endl;

    // 认证用户
    // 新增session -> sessionid
    // 管理 新增session insert sm;

    resp.SetCode(200);
    resp.SetHeader("Cotent-Type", "text/plain");
    resp.SetHeader("Set-Cookie", "status=online; expires=Sat, 20 May 2026 00:00:00 UTC; path=/;");
    resp.SetBody("Login success!\n");
}

void Register(const HttpRequest &req, HttpResponse &resp)
{
    std::string args = req["args"];
    std::cout << "-----> Register service, args: " << args << std::endl;

    resp.SetCode(201);
}

void Search(const HttpRequest &req, HttpResponse &resp)
{
    std::string args = req["args"];
    std::cout << "-----> Search service, args: " << args << std::endl;
}

void CallBigModel(const HttpRequest &req, HttpResponse &resp)
{
    std::string args = req["args"];
    std::cout << "-----> CallBigModel service, args: " << args << std::endl;
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
    hsvr->Register("/app/login", Login);
    hsvr->Register("/app/register", Register);
    hsvr->Register("/app/search", Search);
    hsvr->Register("/app/model", CallBigModel);
    hsvr->Run();

    return 0;
}