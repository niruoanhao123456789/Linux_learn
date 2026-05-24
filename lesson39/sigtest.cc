#include<iostream>
#include<signal.h>
#include<unistd.h>

int cnt = 0;

void handlersig(int sig)
{
    // switch(sig)
    // {
    //     case 1:
    //     break;
    //     case 2:
    //     break;
    //     case 3:
    //     ....
    // }
    std::cout << "正在处理一个信号, pid: " << getpid() << " sig number: " << sig << " cnt: " << cnt << std::endl;
    alarm(2);
    exit(1); // 没有这个exit就会死循环？

    // fork && exec("shutdown ..");
}

int main()
{
    alarm(2); // 一次性闹钟 && 对你这个进程，只会有一个闹钟！
    // int n = alarm(0); // 取消闹钟，返回剩余时间
    // printf("remain: %d\n", n);

    //sleep(100);

    // int a = 10;
    // a /= 0;

    signal(SIGALRM, handlersig);

    while(true)
    {
        std::cout << "counter: " << cnt << std::endl;
        cnt++;
        sleep(1);
    }


    // for (int signo = 1; signo <= 31; signo++)
    //     signal(signo, handlersig);

    // // signal(2, handlersig);
    // // signal(2, SIG_DFL);
    // // signal(2, SIG_IGN);
    // // signal(3, handlersig);
    // // signal(4, handlersig);
    // // signal(11, handlersig);

    // std::cout << "我是一个进程,pid : " << getpid() << std::endl;
    // sleep(2);

    // int *p = nullptr;
    // *p = 100; // 野指针报错

    // // int a = 10;
    // // a /= 0;

    // raise(SIGINT);
    // abort();

    return 0;
}