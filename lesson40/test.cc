#include<iostream>
#include<cstdio>
#include<signal.h>
#include<unistd.h>

void PrintPending(sigset_t& pending)
{
    // bit位
    // 0000 0000... 0000 -> 0000 0000... 0010
    for(int signum=31;signum>=1;signum--)
    {
        if(sigismember(&pending,signum))
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

void handler(int signum)
{
    std::cout<<"get a signal: "<<signum<<std::endl;

    // 在处理2号信号期间，2号信号被自动block了
    while(1)
    {
        sigset_t pending;
        sigpending(&pending);
        PrintPending(pending);
        sleep(1);
    }
    exit(0);
}

int main()
{
    struct sigaction act, old_act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&(act.sa_mask)); //如果进程收到了大量的重复信号？？？
    sigaddset(&(act.sa_mask), 1);
    sigaddset(&(act.sa_mask), 3);
    sigaddset(&(act.sa_mask), 4);
    sigaddset(&(act.sa_mask), 5);

    sigaction(2,&act,&old_act);

    while(1)
        pause();

    return 0;
}


// void PrintPending(sigset_t& pending)
// {
//     // bit位
//     // 0000 0000... 0000 -> 0000 0000... 0010
//     for(int signum=31;signum>=1;signum--)
//     {
//         if(sigismember(&pending,signum))
//             printf("1");
//         else
//             printf("0");
//     }
//     printf("\n");
// }

// void handler(int signo)
// {
//     std::cout<<"处理了： "<<signo<<" 信号"<<std::endl;
//     std::cout<<"----------------------------"<<std::endl;
//     sigset_t pending;
//     sigfillset(&pending);
//     sigpending(&pending);

//     // 打印
//     PrintPending(pending);
//     std::cout<<"----------------------------"<<std::endl;
// }

// int main()
// {
//     // 1、自定义捕捉2号信号
//     signal(SIGINT,handler);

//     std::cout<<"pid: "<<getpid()<<std::endl;
//     // 2、定义与初始化信号集
//     sigset_t block,old_block;
//     sigemptyset(&block);
//     sigemptyset(&old_block);

//     for(int signum=1;signum<=31;signum++)
//         sigaddset(&block,signum);
    
//     // 3、向block信号集添加信号
//     sigaddset(&block,SIGINT); //有没有设置到内核中，有没有设置到当前进程内部？并没有！！！

//     // 4、屏蔽2号信号
//     sigprocmask(SIG_SETMASK,&block,&old_block);

//     // 5、不断获取pending信号集并打印
//     int count = 0;
//     sigset_t pending;
//     while(1)
//     {
//         sigemptyset(&pending);
//         //获取pending信号集
//         sigpending(&pending);

//         // 打印
//         PrintPending(pending);
//         sleep(1);

//         count++;
//         if(count==20)
//         {
//             std::cout<<"恢复2号信号，解除屏蔽"<<std::endl;
//             sigprocmask(SIG_SETMASK,&old_block,nullptr);
//         }
//     }

//     return 0;
// }