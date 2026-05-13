#include<iostream>
#include<cstdio>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include"communication.h"

int main()
{
    // 1、创建管道文件
    umask(0);
    int n = mkfifo(fifoname.c_str(),0666);
    if(n<0)
    {
        perror("mkfifo");
        return 1;
    }

    // 2、打开管道文件
    int rfd = open(fifoname.c_str(),O_RDONLY);
    if(rfd<0)
    {
        perror("open");
        return 2;
    }

    //3、进行通信
    char inbuffer[1024];
    while(1)
    {
        ssize_t n = read(rfd,inbuffer,sizeof(inbuffer)-1);
        if(n>0)
        {
            inbuffer[n]={0};
            std::cout<<"client say# "<<inbuffer<<std::endl;
        }
        else if(n==0)
        {
            // 写端关闭
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }

    // 4、关闭
    close(rfd);

    //5、删除管道文件
    unlink(fifoname.c_str());

    return 0;
}