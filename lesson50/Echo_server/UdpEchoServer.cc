#include"UdpEchoServer.hpp"

void Usage(const std::string& name)
{
    std::cerr<<"Usage: "<<name<<" ip port"<<std::endl;
}

// ./UdpEchoServer 8080
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        exit(0);
    }

    uint16_t server_port = std::stoi(argv[1]);

    UdpEchoServer usvr(server_port);
    usvr.Init();
    usvr.Start();

    return 0;
}