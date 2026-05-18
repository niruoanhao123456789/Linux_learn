#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int exit_code = 0;

int main()
{
    printf("这里是父进程: pid: %d, ppid: %d\n",getpid(),getppid());
    pid_t id = fork();
    if(id<0)
    {
        perror("fork");
        exit(1);
    }
    else if(id==0)
    {
        int cnt = 3;
        while(cnt>=0)
        {
            printf("这里是子进程: pid: %d, ppid: %d, cnt: %d\n",getpid(),getppid(),cnt);
            sleep(1);
            cnt--;
        }
        printf("子进程退出\n");
        exit_code = 11;
        exit(11);
    }

    int status = 0;
    pid_t rid = waitpid(id,&status,0);
    if(rid>0)
    {
        printf("等待子进程成功, status: %d, exit code: %d\n",status,(status>>8)&0xFF);
    }

    return 0;
}