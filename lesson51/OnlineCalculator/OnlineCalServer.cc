#include"Calculator.hpp"
#include"Protocol.hpp"
#include"TcpServer.hpp"
#include"Daemon.hpp"
using namespace LogModule;

void Usage(std::string procname)
{
    std::cout<<"Usage: "<<procname<<" local_port"<<std::endl;
}

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        exit(1);
    }

    Daemon(1,1); 
    // daemon(0, 0);

    uint16_t port = std::stoi(argv[1]);
    // ENABLE_CONSOLE_LOG_STRATEGY();
    ENABLE_FILE_LOG_STRATEGY();

    // 业务层
    std::unique_ptr<Calculator> cal = std::make_unique<Calculator>();

    // 协议层
    std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>([&cal](const Request& req)->Response
                                                                    {return cal->Exec(req);});
    // 网络通信层
    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(port,[&protocol](std::string& streamer) -> std::string
                                                                    {return protocol->HandlerRequest(streamer);});
    tsvr->Run();
                                                            
    return 0;
}   