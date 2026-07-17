#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"InetAddr.hpp"

void Usage(std::string procname)
{
    std::cout << "Usage: " << procname << " serverip serverport" << std::endl;
}

// ./TcpEchoClient serverip serverport
int main(int argc,char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);

    // 1. 创建tcp socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        std::cerr << "socket error" << std::endl;
        exit(2);
    }

    // 2. client需要明确的进行bind么？不需要显示绑定。
    //    client必须要有socket信息，即自己的ip+port，一定需要bind，不用用户bind，OS自动随机bind的！
    //    OS 什么时候bind？
    //    建立连接。a. 首次建立连接的时候，client会自动bind自己的socket信息 b. 向{serverip,serverport}发起建立连接的请求
    // Tcp 通信，是面向连接的.
    InetAddr serveraddress(server_port,server_ip);
    int n = connect(sockfd,serveraddress.Addr(),serveraddress.AddrLen());
    if(n<0)
    {
        std::cerr<<"connect error"<<std::endl;
        exit(3);
    }    

    // 3. sockfd 通信过程
    while(1)
    {
        std::string line;
        std::cout<<"Please Enter# ";
        std::getline(std::cin,line);

        // TCP sockfd是全双共的
        // 发送数据
        ssize_t n = write(sockfd,line.c_str(),line.size());

        char buffer[4096]={0};
        n = read(sockfd,buffer,sizeof(buffer)-1);
        if(n > 0)
        {
            buffer[n] = 0;
            std::cout << "-> " << buffer << std::endl;
        }
        else if(n == 0)
        {
            std::cerr << "server quit" << std::endl;
            break;
        }
        else{
            std::cerr << "read error" << std::endl;
            break;
        }
    }
    close(sockfd);

    return 0;
}