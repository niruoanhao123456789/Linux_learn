#include<iostream>
#include<signal.h>
#include<unistd.h>

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
    std::cout <<"正在处理一个信号, pid: " << getpid() << " sig number: " << sig << std::endl;
}

int main()
{
    for (int signo = 1; signo <= 31; signo++)
        signal(signo, handlersig);

    // signal(2, handlersig);
    // signal(2, SIG_DFL);
    // signal(2, SIG_IGN);
    // signal(3, handlersig);
    // signal(4, handlersig);
    // signal(11, handlersig);

    std::cout << "我是一个进程,pid : " << getpid() << std::endl;
    sleep(2);

    int *p = nullptr;
    *p = 100; // 野指针报错

    // int a = 10;
    // a /= 0;

    raise(SIGINT);
    abort();

    return 0;
}