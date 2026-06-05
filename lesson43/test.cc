#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<sched.h>
#include<sys/wait.h>
#include<signal.h>

int g_val = 100;
int *p = nullptr;

void hello(const std::string &name)
{
    printf("I am common funciton!, %s\n", name.c_str());
    sleep(5);
}

class Task
{
    public:
    void operator()()
    {
        printf("tackling task\n");
    }
    private:
};

void* threadrun1(void* args)
{
    std::string threadname = static_cast<const char*>(args);
    while(1)
    {
        std::cout<<threadname<<std::endl;
        sleep(1);
        printf("%s isrunning, g_val: %d, &g_val: %p\n", threadname.c_str(), g_val, &g_val);
        sleep(1);
        hello(threadname);
    }
}

void* threadrun2(void* args)
{
    std::string threadname = static_cast<const char*>(args);
    while(1)
    {
        std::cout<<threadname<<std::endl;
        sleep(1);
        printf("%s isrunning, g_val: %d, &g_val: %p\n", threadname.c_str(), g_val, &g_val);
        sleep(1);
        g_val++;
        hello(threadname);
    }
}

int main()
{
    pthread_t t1,t2;
    // 给线程传递类或者结构体变量
    Task t;
    pthread_create(&t1,nullptr,threadrun1,(void*)&t);
    pthread_create(&t2,nullptr,threadrun2,(void*)"thread-2");

    while(1)
    {
        printf("Main thread, thread1 id: %p, thread2 id: %p\n",t1,t2);
        sleep(1);
    }

    return 0;
}



// // 子线程函数（LWP执行函数）
// int thread_fun(void* arg)
// {
//     int thread_id = *(int *)arg;

//     printf("LWP %d (PID: %d, 父进程: %d) 开始执行\n",thread_id,getpid(),getppid());

//     for(int i = 1;i<=5;i++)
//     {
//         printf("LWP %d 计数：%d\n",thread_id,i);
//         sleep(1);
//     }

//     printf("LWP %d 执行完成\n",thread_id);
//     return thread_id * 10; // 返回退出码
// }

// int main()
// {
//     const int  MUN_THREADS = 5;
//     void** stacks;              // 栈空间指针数组
//     int* thread_ids;            // 线程ID数组
//     pid_t* pids;                // LWP的PID数组
//     int stack_size = 1024*1024; // 每个栈分配1MB

//     // 分配内存
//     stacks = (void**)malloc(MUN_THREADS * sizeof(void*));
//     thread_ids = (int*)malloc(MUN_THREADS * sizeof(int*));
//     pids = (pid_t*)malloc(MUN_THREADS*sizeof(pid_t));

//     if(!stacks||!thread_ids||!pids)
//     {
//         perror("malloc");
//         exit(1);
//     }

//     printf("父进程 PID: %d\n\n",getpid());

//     // 创建5个LWP
//     for(int i=0;i<MUN_THREADS;i++)
//     {
//         // 为每个LWP分配独立的栈空间（向下增长，分配在堆上）
//         stacks[i]=malloc(stack_size);
//         if(!stacks[i])
//         {
//             perror("栈分配失败");
//             exit(1);
//         }
//         thread_ids[i]=i+1;

//         // 使用clone创建LWP
//         // CLONE_VM:      共享内存空间（父子进程共享地址空间）
//         // CLONE_FS:      共享文件系统信息
//         // CLONE_FILES:   共享文件描述符表
//         // CLONE_SIGHAND: 共享信号处理器
//         // CLONE_THREAD:  放入同一线程组（与父进程共享PID）
//         // SIGCHLD:       子进程退出时发送信号
//         pids[i]=clone(thread_fun,
//             stacks[i]+stack_size,
//             CLONE_VM|CLONE_FS|CLONE_FILES|
//             CLONE_SIGHAND|CLONE_THREAD|SIGCHLD,
//             &thread_ids[i]);
        
//         if(pids[i]==-1)
//         {
//             perror("clone失败");
//             exit(1);
//         } 
//         printf("创建 LWP %d, LWP ID: %d\n",thread_ids[i],pids[i]);
//     }

//     printf("\n所有LWP已创建，等待执行完成...\n");
//     printf("注意：由于共享内存，主进程需要等待一段时间\n\n");

//     // 注意：因为使用了CLONE_THREAD，不能使用waitpid等待
//     // 简单的做法是等待足够长时间让子线程完成
//     sleep(8);

//     printf("\n主进程结束，清理资源\n");

//     // 清理栈空间
//     for(int i=0;i<MUN_THREADS;i++)
//     {
//         free(stacks[i]);
//     }
//     free(stacks);
//     free(thread_ids);
//     free(pids);

//     return 0;
// }