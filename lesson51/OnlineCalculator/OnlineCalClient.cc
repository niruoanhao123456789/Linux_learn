#include"Protocol.hpp"
#include"Socket.hpp"
#include<iostream>
#include<memory>

void Usage(std::string procname)
{
    std::cout<<"Usage: "<<procname<<" server_ip server_port"<<std::endl;
}

int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        Usage(argv[0]);
        exit(1);
    }

    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);

    std::unique_ptr<Socket> sockfd = std::make_unique<TcpSocket>();
    sockfd->BuildClientSocketMethod(server_ip,server_port);

    std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>();
    std::string inbuffer;
    while(1)
    {
        int count = 3;
        std::string sendstring;
        while(count--)
        {
            // 构建请求
            Request req;
            std::cout<<"Enter x # ";
            std::cin>>req._x;
            std::cout<<"Enter y # ";
            std::cin>>req._y;
            std::cout<<"Enter oper # ";
            std::cin>>req._oper;

            // 序列化
            std::string reqjsonstr;
            req.Serialize(&reqjsonstr);

            // 封装报头
            sendstring += protocol->Package(reqjsonstr);
        }
        std::cout<<sendstring<<std::endl;

        // 发送
        sockfd->Send(sendstring);

        // 接受应答
        sockfd->Recv(&inbuffer); // S能保证读到的是完整的报文吗？
        
        // 解包并判断
        while(1)
        {
            std::string jsonrespstr;
            int n = protocol->UnPackage(inbuffer,&jsonrespstr);
            if(n==0)
            {
                LOG(LogLevel::DEBUG)<<"解包完成";
                break;
            }

            // 反序列化
            Response resp;
            resp.DeSerialize(jsonrespstr);

            // 显示结果
            std::cout<<resp._result << "["<<resp._exitcode<<"]"<<std::endl;
        }

    }

    return 0;
}