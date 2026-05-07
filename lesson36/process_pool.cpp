#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<memory>
#include<functional>
#include<unistd.h>
#include<sys/wait.h>

// 先描述
class Channel
{
public:
    Channel(int wfd,pid_t who)
    :_wfd(wfd)
    ,_sub_process_id(who)
    {
        _name = "Channel-" + std::to_string(_sub_process_id) + std::to_string(_wfd);
    }

    int fd()
    {
        return _wfd;
    }

    pid_t sub_id()
    {
        return _sub_process_id;
    }

    std::string name()
    {
        return _name;
    }

    void Close()
    {
        if(_wfd>=0)
            close(_wfd);
    }

    void Wait()
    {
        pid_t rid = waitpid(_sub_process_id,nullptr,0);
    }

    void SendTask(int taskcode)
    {
        ssize_t n = write(_wfd,&taskcode,sizeof(taskcode));
    }

private:
    int _wfd;
    pid_t _sub_process_id;
    std::string _name; 
}

class ProcessPool
{
private:
    int Next()
    {
        int choice = _next_choice;
        _next_choice++;
        _next_choice %= _channels.size();
    }

public:
    ProcessPool(int number)
    :_number(number)
    ,_next_choice(0)
    {
        std::cout<<"Number:"<<_number<<std::endl;
    }

    // 父进程
    void Start()
    {
        for(int i = 0;i < _number;i++)
        {
            // 1、创建管道
            int pipefd[2]={0};
            int n = pipe(pipefd);
            if(n<0)
            {
                perror("pipe");
                exit(2);
            }
            // 2、创建子进程
            pid_t id = fork();
            if(id<0)
            {
                perror("fork");
                exit(3);
            }
            else if(id==0) // 子进程
            {
                // 关闭父进程历史的wfd
                for(auto& channel:_channels)
                    channel.close();

                close(pipefd[1]);
                Work(pipefd[0]);
                close(pipefd[0]);
                exit(0);
            }
            else // 父进程
            {
                close(pipfd[0]);

                _channels.emplace_back(pipefd[1],id);
            }
        }
    }

    // 1、是什么任务？由任务码决定
    // 2、任务给谁？属于进程池内部操作，采用负载均衡
    void PushTask(int taskcode)
    {
        // 选择一个子进程 用于交付任务
        int who = Next();
        _channels[who].SendTask(taskcode);
    }

private:
    std::vector<Channel> _channels;
    int _number;
    int _next_choice;
}