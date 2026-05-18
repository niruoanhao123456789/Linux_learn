#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

///////////////////////任务测试代码/////////////////////////
using task_t = std::function<void()>;

void printlog()
{
    // sleep(1);
    std::cout << "我是一个打印日志的任务, pid: " << getpid() << std::endl;
}

void download()
{
    // sleep(1);
    std::cout << "我是一个下载任务, pid: " << getpid() << std::endl;
}

void readmysql()
{
    // sleep(1);
    std::cout << "我是一个访问数据库的任务, pid: " << getpid() << std::endl;
}

void writeredis()
{
    // sleep(1);
    std::cout << "我是一个访问redis的任务, pid: " << getpid() << std::endl;
}

std::vector<task_t> gtasks;

void LoadTask()
{
    gtasks.push_back(printlog);
    gtasks.push_back(download);
    gtasks.push_back(readmysql);
    gtasks.push_back(writeredis);
}
// *: 输出型参数
// const &: 输入型参数
// &: 输入输出型
void RandomTask(std::vector<int> *out)
{
    for (int i = 0; i < 50; i++)
    {
        int code = rand() % gtasks.size();
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
    switch (code)
    {
    case LOG_TASK:
        return "printlog";
    case DOWNLOAD_TASK:
        return "download";
    case MYSQL_TASK:
        return "readmysql";
    case REDIS_TASK:
        return "writeredis";
    default:
        return "unknown";
    }
}

///////////////////////进程池代码/////////////////////////

void Work(int rfd)
{
    while (true)
    {
        int code = 0;
        ssize_t n = read(rfd, &code, sizeof(code));
        if (n == sizeof(int))
        {
            if (code >= 0 && code < gtasks.size())
            {
                gtasks[code]();
            }
        }
        else if (n == 0)
        {
            break; // 子进程只要读到返回值为0, 表明父进程让我退出
        }
        else
        {
            break;
        }
    }
}

class Channel
{
public:
    Channel(int wfd, pid_t who) : _wfd(wfd), _sub_process_id(who)
    {
        _name = "Channel-" + std::to_string(_sub_process_id) + "-" + std::to_string(_wfd);
    }
    int Fd()
    {
        return _wfd;
    }
    pid_t SubId()
    {
        return _sub_process_id;
    }
    std::string Name()
    {
        return _name;
    }
    void Close()
    {
        if (_wfd >= 0)
            close(_wfd);
    }
    void Wait()
    {
        pid_t rid = waitpid(_sub_process_id, nullptr, 0);
        (void)rid;
    }
    void SendTask(int taskcode)
    {
        ssize_t n = write(_wfd, &taskcode, sizeof(taskcode));
        (void)n;
    }
    ~Channel()
    {
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
        return choice;
    }

public:
    ProcessPool(int number) : _number(number), _next_choice(0)
    {
        std::cout << "number: " << _number << std::endl;
    }
    // 父进程
    void Start()
    {
        for (int i = 0; i < _number; i++)
        {
            // 1. 创建管道
            int pipefd[2];
            int n = pipe(pipefd);
            if (n < 0)
            {
                perror("pipe");
                exit(2);
            }
            // 2. 创建子进程
            pid_t id = fork();
            if (id < 0)
            {
                perror("fork");
                exit(3);
            }
            else if (id == 0) // 子进程
            {
                // 关闭父进程历史的wfd！
                for(auto &channel : _channels)
                    channel.Close();

                close(pipefd[1]);
                Work(pipefd[0]);
                close(pipefd[0]);
                exit(0);
            }
            else // 父进程
            {
                close(pipefd[0]);
                // pipefd[1];
                _channels.emplace_back(pipefd[1], id);
            }
        }
    }
    // 1. 什么任务？任务码决定
    // 2. 任务给谁？属于进程池内部操作，负载均衡
    void PushTask(int taskcode)
    {
        // 选择一个子进程
        int who = Next();
        _channels[who].SendTask(taskcode);

        std::cout << "发送任务: " << TaskToString(taskcode) << "[" << taskcode << "]" << " 给: " << _channels[who].Name() << std::endl;
    }
    void Stop()
    {
        // version 2 ???
        for(auto &channel: _channels)
        {
            channel.Close();
            channel.Wait();
            std::cout << channel.Name() << " close and wait success!" << std::endl;
        }

        // version3
        // int end = _channels.size() - 1;
        // while(end >= 0)
        // {
        //     _channels[end].Close();
        //     _channels[end].Wait();
        //     std::cout <<  _channels[end].Name() << " closea and wait success!" << std::endl;
        //     end--;
        // }

        // 内部bug！
        // 1. 关闭wfd -- version1
        // for (auto &channel : _channels)
        // {
        //     channel.Close();
        //     std::cout << channel.Name() << " close success!" << std::endl;
        // }

        // // sleep(3);
        // // // 2. 回收子进程
        // for (auto &channel : _channels)
        // {
        //     channel.Wait();
        //     std::cout << channel.Name() << " wait success!" << std::endl;
        // }
    }
    void DebugPrint()
    {
        std::cout << "-------------------------------------" << std::endl;
        for (auto &channel : _channels)
        {
            std::cout << channel.Fd() << std::endl;
            std::cout << channel.SubId() << std::endl;
            std::cout << channel.Name() << std::endl;
        }
        std::cout << "-------------------------------------" << std::endl;
    }
    ~ProcessPool() {}

private:
    std::vector<Channel> _channels;
    int _number;
    int _next_choice;
};

// 父进程

static void Usage(const std::string &proc)
{
    std::cout << "Usage:\n\t" << proc << " process_number" << std::endl;
}

// ./process_pool 5
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    int number = std::stoi(argv[1]);
    // 0. 加载任务
    srand(time(nullptr) ^ getpid());
    LoadTask();
    std::vector<int> task_codes;
    RandomTask(&task_codes);

    // 1. 创建进程池对象
    std::unique_ptr<ProcessPool> pp = std::make_unique<ProcessPool>(number);
    // 2. 启动进程池
    pp->Start();
    sleep(2);
    for (auto task : task_codes)
    {
        pp->PushTask(task);
        usleep(500000);
    }

    // while(true)
    // {
    //     int code = 0;
    //     std::cout << "Please Enter Your Task# ";
    //     std::cin >> code;
    //     if(code < 0 || code > gtasks.size())
    //     {
    //         std::cout << "任务码错误, 请重新输入"<< std::endl;
    //         continue;
    //     }

    //     pp->PushTask(code);
    // }

    pp->Stop();
    return 0;
}