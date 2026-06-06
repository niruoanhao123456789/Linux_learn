#include<iostream>
#include<cstdio>
#include<vector>
#include<string>
#include<unistd.h>
#include<pthread.h>
#include<sys/syscall.h>
#include"Task.hpp"

__thread int g_cnt = 1;
__thread pid_t lwp = 0;

pid_t getlwp()
{
    return syscall(SYS_gettid);
}

void hello()
{
    printf("tid: %d\n",lwp);
}

void* threadrun(void* args)
{
    printf("threadrun new thread tid=0x%lx\n", pthread_self());
    pthread_detach(pthread_self());
    std::string name = static_cast<const char*>(args);
    int cnt =3;
    lwp = getlwp();
    while(1)
    {
        printf("%s, g_cnt: %d, &g_cnt: %p, lwp: %d\n", name.c_str(), g_cnt, &g_cnt, lwp);
        g_cnt++;
        cnt--;
        sleep(1);
        hello();
    }
    std::cout << name << " is  quit..." << std::endl;
    return nullptr;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid,nullptr,threadrun,(void*)"thread-1");
    printf("new thread tid=0x%lx\n", tid);
    printf("main thread tid=0x%lx\n", pthread_self());
    lwp = getlwp();

    while(1)
    {
        printf("%s, g_cnt: %d, &g_cnt: %p, lwp: %d\n","main-thread",g_cnt,&g_cnt,lwp);
        sleep(1);
        hello();
    }
    int n = pthread_join(tid,nullptr);
    std::cout<<"main thread, n = "<<n<<std::endl;

    return 0;
}


// const int gsize = 64;

// void* threadrun(void* args)
// {
//     std::string name =  static_cast<const char*>(args);
//     int cnt = 5;
//     while(1)
//     {
//         printf("这是一个新线程: tid: 0x%lx, pid: %d, name: %s, cnt: %d\n",
//         pthread_self(),getpid(),name.c_str(),cnt);
//         cnt--;
//         sleep(1);
//         // pthread_cancel(pthread_self());
//         // return nullptr;
//         // pthread_exit(nullptr);
//     }
//     // return (void*)10;// 1 2 3 4
//     pthread_exit((void*)100);
//     // return nullptr;
//     // 新线程退出：
//     // 只要自己的线程函数跑完，线程自然就退出了 -- 调用return，表示线程退出
//     // return nullptr;
//     // pthread_exit(nullptr);
//     // exit(3); // 不能用来终止线程，它是用来终止进程的！，多线程中，任意一个线程调用exit，都表示整个进程退出
// }

// // 任何线程异常，都会导致进程退出，join异常无法正确返回，没有意义！！
// int main()
// {
//     pthread_t tid;
//     char threadname[gsize];
//     snprintf(threadname,gsize,"thread-%d",1);

//     pthread_create(&tid,nullptr,threadrun,(void*)threadname);

//     // PTHREAD_CANCELLED
//     sleep(1);
//     int n = pthread_cancel(tid);
//     printf("cancel new thread done, n : %d\n",n);

//     // 如果线程异常了呢？不关心！
//     void* ret = nullptr;
//     pthread_join(tid,&ret); // 如果新线程不退出，join: 一直阻塞等待下去

//     printf("join %lx success, ret code: %lld\n", tid, (long long)ret);

//     return 0;
// }


// const int gsize = 64;

// // void *threadrun(void *args)
// // {
// //     // Task *t = static_cast<Task *>(args);
// //     // sleep(1);
// //     // (*t)();
// //     // sleep(1);
// //     int cnt = 5;
// //     while (cnt--)
// //     {
// //         sleep(1);
// //         // printf("我是一个新线程: tid: 0x%lx, pid: %d, name : %s\n", pthread_self(), getpid(), name.c_str());
// //         // sleep(1);
// //     }

// //     return nullptr;
// // }

// void* threadrun(void* args)
// {
//     Task* t = static_cast<Task*>(args);
//     t->Execute();
//     return t;
// }

// // ./createThread num
// int main(int argc,char* argv[])
// {
//     if(argc!=2)
//     {
//         std::cout<<argv[0]<<" num"<<std::endl;
//         return 1;
//     }

//     int num = std::stoi(argv[1]);
//     std::vector<pthread_t> tids;
//     for(int i=0;i<num;i++)
//     {
//         // 如果要创建多线程
//         pthread_t tid;
//         char threadname[gsize];
//         snprintf(threadname,gsize,"thread-%d",i+1);

//         Task* t = new Task(threadname,10+i,20*i);

//         pthread_create(&tid,nullptr,threadrun,t);
//         tids.push_back(tid);
//         std::cout<<"create thread "<<threadname<<" done"<<std::endl;
//         sleep(1);
//     }

//     std::vector<Task*> result_list;
//     for(auto& tid:tids)
//     {
//         Task* t = nullptr;
//         pthread_join(tid,(void**)&t);
//         result_list.push_back(t);
//         std::cout<<"join success: "<<tid<<std::endl;
//     }

//     std::cout<<"处理结果清单："<<std::endl;
//     for(auto&res:result_list)
//     {
//         std::cout<<res->Result()<<std::endl;
//     }

//     return 0;
// }