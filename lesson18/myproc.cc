#include<iostream>
#include<cstdio>
#include<unistd.h>

int main(int argc,char* argv[],char* env[])
{
    printf("---------------------------------------\n");
    std::cout<<"这是个c++程序，先变成了一个进程："<<getpid()<<std::endl;
    for(int i=0;i<argc;i++)
    {
        printf("argv[%d]: %s\n",i,argv[i]);
    }
    printf("---------------------------------------\n");
    for(int i=0;env[i];i++)
    {
        printf("env[%d]: %s\n",i,env[i]);
    }
    printf("---------------------------------------\n");

    return 0;
}