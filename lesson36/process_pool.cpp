#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<memory>
#include<functional>
#include<unistd.h>
#include<sys/wait.h>

// 任务测试代码
using task_t = std::function<void()>;

void printlog()
{
    std::cout<<"This is a task about log print, pid:"<<getpid()<<std::endl;
}

void download()
{
    std::cout<<"This is a task about download, pid:"<<getpid()<<std::endl;
}

void readmysql()
{
    std::cout<<"This is a task about reading mysql, pid"<<getpid()<<std::endl;
}

void writeredis()
{
    std::cout<<"This is a task about reading redis, pid:"<<getpid()<<std::endl;
}

std::vector<task_t> gtasks;

void LoadTask()
{
    gtasks.push_back(printlog);
    gtasks.push_back(download);
    gtasks.push_back(readmysql);
    gtasks.push_back(writeredis);
}

//* 输出型参数
//const& 输入型参数
//& 输入输出型参数
void RandomTask(std::vector<int>* out)
{
    for(int i=0;i<50;i++)
    {
        int code = rand()%gtasks.size();
        usleep(23223);
        out->push_back(code);
    }
}

#define LOG_TASK 0
#define DOWNLOAD_TASK 1
#define MYSQL_TASK 2
#define REDIS_TASK 3

std::string TaskToString(int code)
{
    switch(code)
    {
    case LOG_TASK:
        return "printlog";
    case DOWNLOAD_TASK:
        return "download";
    case MYSQL_TASK:
        return "readymysql";
    case REDIS_TASK:
        return "writeredis";
    default:
        return "unkonwn";
    }
}

//进程池代码部分

void Work(int rfd)
{
    while(1)
    {
        int code = 0;
        ssize_t n = read(rfd,&code,sizeof(code));
        if(n == sizeof(int))
        {
            if(code>=0 && code<gtasks.size())
                gtasks[code]();
        }
        else if(n==0)
            break; // 子进程读取到返回值为0，表明父进程让其退出
        else
            break;
    }
}

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

    std::string Name()
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
};

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
                    channel.Close();

                close(pipefd[1]);
                Work(pipefd[0]);
                close(pipefd[0]);
                exit(0);
            }
            else // 父进程
            {
                close(pipefd[0]);

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
        std::cout<<"发送任务："<<TaskToString(taskcode)<<"["<<taskcode<<"]"<<"给"<<_channels[who].Name()<<std::endl;
    }

    void Stop()
    {
        // // version1 存在内部bug
        // // 1、关闭wfd 
        // for(auto& channel:_channels)
        // {
        //     channel.Close();
        //     std::cout<<channel.Name()<<"close success!"<<std::endl;
        // }
        // //sleep(3);
        // // 2、回收子进程
        // for(auto& channel:_channels)
        // {
        //     channel.Wait();
        //     std::cout<<channel.Name()<<"wait success!"<<std::endl;
        // }

        // version2
        for(auto& channel:_channels)
        {
            channel.Close();
            channel.Wait();
            std::cout<<channel.Name()<<"close and wait sucess!"<<std::endl;
        }

        // // version3
        // int end = _channels.size()-1;
        // while(end>=0)
        // {
        //     _channels[end].Close();
        //     _channels[end].Wait();
        //     std::cout<<_channels[end].Name()<<"close and wait success!"<<std::endl;
        //     end--;
        // }
    }

    void DebugPrint()
    {
        std::cout<<std::endl;
        std::cout<<"There are information of Debug"<<std::endl;
        for(auto& channel:_channels)
        {
            std::cout<<channel.fd()<<std::endl;
            std::cout<<channel.sub_id()<<std::endl;
             std::cout<<channel.Name()<<std::endl;
        }
        std::cout<<std::endl;
    }

private:
    std::vector<Channel> _channels;
    int _number;
    int _next_choice;
};


// 父进程

static void Usage(const std::string& proc)
{
    std::cout << "Usage:\n\t" << proc << " process_number" << std::endl;
}

int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        exit(1);
    }
    int number = std::stoi(argv[1]);

    // 加载任务
    srand(time(nullptr) ^ getpid());
    LoadTask();
    std::vector<int> task_codes;
    RandomTask(&task_codes);

    // 创建进程池对象
    std::unique_ptr<ProcessPool> pp = std::make_unique<ProcessPool>(number);
    // 启动进程池
    pp->Start();
    sleep(2);

    for (auto task : task_codes)
    {
        pp->PushTask(task);
        usleep(500000);
    }

    while(true)
    {
        int code = 0;
        std::cout << "Please Enter Your Task# ";
        std::cin >> code;
        if(code < 0 || code > gtasks.size())
        {
            std::cout << "任务码错误, 请重新输入"<< std::endl;
            continue;
        }

        pp->PushTask(code);
    }

    pp->Stop();

    return 0;
}