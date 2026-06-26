#include<iostream>
#include<string>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

void Usage(const std::string& name)
{
    std::cerr << "Usage: " << name << " server_ip server_port" << std::endl;
}

// 作为client端，它怎么知道服务器端的IP和port？
// ./udpechoclient xxx.x.x.x 8080
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        Usage(argv[0]);
        exit(0);
    }
    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);

    // 1、创建udp socket
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd < 0)
    {
        std::cerr << "socket error" << std::endl;
    }

    // 2. 构建目标服务器socket信息
    //    自己一定需要自己的IP和端口号。
    //    但是，client不能自己显示的bind port，一般客户端都是由OS自己选择IP和Port，
    //    尤其是Port，client的port要让OS随机选择
    //    客户端port，是多少不重要，唯一才重要
    //    服务器port，是多少很重要，唯一是基础
    //    client不能自己显示的bind port, 但是必须bind，由OS自己完成，Port随机
    struct sockaddr_in server;
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());

    std::string inbuffer;
    while(1)
    {
        std::cout<<"Please Enter# ";
        std::cin>>inbuffer;

        // 1、发送数据
        ssize_t n = sendto(sockfd,inbuffer.c_str(),inbuffer.size(),0,(struct sockaddr*)&server,sizeof(server));
        (void)n;

        // 2、接收数据
        struct sockaddr_in tmp;
        socklen_t len = sizeof(tmp);
        char buffer[1024];
        ssize_t m = recvfrom(sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&tmp,&len);
        if(m>0)
            buffer[m]=0;
        std::cout<<buffer<<std::endl;
    }

    return 0;
}