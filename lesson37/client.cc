#include<iostream>
#include<string>
#include<cstdio>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include"communication.h"

int main()
{
    int wfd = open(fifoname.c_str(),O_WRONLY);
    if(wfd<0)
    {
        perror("open");
        return 1;
    }

    std::string outstring;
    while(1)
    {
        std::cout<<"Please Enter@ ";
        std::cin>>outstring;
        write(wfd,outstring.c_str(),outstring.size());
    }

    close(wfd);

    return 0;
}