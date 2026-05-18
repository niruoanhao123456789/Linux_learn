#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main()
{
    printf("这里是父进程：pid: %d, ppid: %d\n",getpid(),getppid());
    pid_t id = fork();
    if(id<0)
    {
        perror("fork");
        exit(1);
    }
    else if(id==0)
    {
        printf("这里是子进程：pid: %d, ppid: %d\n",getpid(),getppid());
        sleep(1);
        char* const argv[] = {
            (char*)"top",
            (char*)"-d",
            (char*)"1",
            (char*)"-n",
            (char*)"3",
            NULL
        };

        char* const env[] = {
            (char*)"haha==hehe",
            (char*)"HOME=/home",
            (char*)"BASH==XXX",
            (char*)"PATH=/user/bin/",
            NULL
        };

        //execvp(argv[0],argv);
        //execlp("ls", "ls", "-a", "-l", "-n", NULL);
        //execv("/usr/bin/top", argv);
        //execl("/usr/bin/ls", "ls", "-a", "-l", NULL);
        //execl("/usr/bin/top", "top", "-d", "1", "-n", "3", NULL);
        execl("./myproc", "myproc", "-a", "-b", "-c", NULL); //我们没有传递环境变量！
        //execle("./myproc", "myproc", "-a", "-b", "-c", NULL, env); //我们没有传递环境变量！
        // extern char **environ;
        // putenv((char*)"haha=hehe");
        // putenv((char*)"class=118");
        // execle("./myproc", "myproc", "-a", "-b", "-c", NULL, environ);
        //execl("/usr/bin/bash", "bash", "shell.sh", NULL);
        //execl("/usr/bin/python3", "python3", "test.py", NULL);
        exit(1);

    }
    else
    {
        pid_t rid = waitpid(id,NULL,0);
        if(rid>0)
        {
            printf("wait success\n");
        }
    }

    return 0;
}