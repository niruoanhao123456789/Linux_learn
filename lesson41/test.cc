#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

// void Schedule()
// {
//     printf(" 调度中......\n");
// }
//
// void IDT(int signum)
// {
//     switch (signum)
//     {
//     case 1:
//         break;
//     case 2:
//         Schedule();
//         break;
//     case 3:
//         break;
//     case 4:
//         break;
//     case 5:
//         break;
//     case 6:
//         break;
//     case 7:
//         break;
//     case 8:
//         break;
//     case 9:
//         break;
//         // ....
//     }
// }
//
// int main()
// {
//     for(int i=1;i<=31;i++)
//     {
//         signal(i,IDT);
//     }
//     while(1)
//     {
//         pause();
//     }

//     return 0;
// }


// volatile int flag = 0;

// void handler(int signum)
// {
//     flag = 1;
//     printf("更改flag 0->1\n");
// }

// int main()
// {
//     signal(2,handler);
//     while(!flag);

//     printf("进程正常退出\n");

//     return 0;
// }


void WaitChild(int signo)
{
    printf("pid: %d get a signal: %d\n",getpid(),signo);
    sleep(5);
    while(1)
    {
        pid_t rid = waitpid(-1,nullptr,WNOHANG);
        if(rid>0)
        {
            printf("wait child: %d success\n",rid);
        }
        else
        {
            break;
        }
    }
}

int main()
{
    signal(SIGCHLD,WaitChild);
    //signal(SIGCHLD,SIG_IGN);

    for(int i=0;i<10;i++)
    {
        pid_t id = fork();
        if(id == 0)
        {
            int count = 5;
            while(count--)
            {
                printf("我是子进程: pid: %d, ppid: %d\n",getpid(),getppid());
                sleep(1);
            }
            printf("子进程退出\n");
            exit(0);
        }
    }

    while(1)
    {
        printf("父进程任务完成\n");
        sleep(1);
    }

    // while(1)
    // {
    //     pause();
    // }

    return 0;
}