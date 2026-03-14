#include<iostream>
#include<string>
#include<unistd.h>
using namespace std;

void WriteData(int wfd)
{
    int cnt = 1;
    pid_t id = getpid();
    while(1)
    {
        sleep(1);
        string message = "hello father process! my pid is:"+to_string(id)+" cnt:"+to_string(cnt++);
        write(wfd,message.c_str(),message.size());
    }
}

void ReadData(int rfd)
{
    char inbuffer[1024]={0};
    while(1)
    {
        sleep(5);
        size_t n = read(rfd,inbuffer,sizeof(inbuffer)-1);
        if(n>0)
        {
            inbuffer[n]='\0';
            cout<<getpid()<<"# "<<inbuffer<<endl;
        }
    }
}

int main()
{
    // 创建管道
    int pipefd[2]={0};
    pipe(pipefd);

    // 创建子进程
    pid_t pid = fork();
    if(pid==0)
    {
        // 3. 形成单向通信的信道
        // 子进程:w
        close(pipefd[0]);
        WriteData(pipefd[1]);
        close(pipefd[1]);
        exit(0);

    }
    else if(pid>0)
    {
        // 父进程:r
        close(pipefd[1]);
        ReadData(pipefd[0]);
        close(pipefd[0]);
        exit(0);
    }
    else
    {
        perror("fork:");
        exit(1);
    }

    return 0;
}