#pragma once
#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>

// 将进程变成守护进程
void Daemon(int changeToRoot,int redir)
{
    // 1、切换工作路径
    if(changeToRoot)
        chdir("/");
    
    // 2、忽略相关信号
    signal(SIGPIPE,SIG_IGN);
    signal(SIGCHLD,SIG_IGN);

    // 3、让自己不要成为所处组组长
    if(fork()>0)
    {
        // 父进程
        exit(0);
    }

    // 子进程
    // 4、形成新会话，自己是话首进程
    setsid();

    // 5、处理标准输入输出
    if(redir)
    {
        int fd = open("/dev/null",O_RDWR);
        if(fd>=0)
        {
            dup2(fd,0);
            dup2(fd,1);
            dup2(fd,2);
        }
        close(fd);
    }
}