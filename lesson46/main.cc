#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<pthread.h>
#include<unistd.h>
#include"Thread.hpp"
#include <chrono>

int ticket = 1000;

typedef struct threadData
{
    std::string name;
    pthread_mutex_t* plock;
}threaddata_t;

void* route(void* args)
{
    threaddata_t* td = static_cast<threaddata_t*>(args);
    while(1)
    {
        // pthread_mutex_lock(td->plock);
        if(ticket>0)
        {
            usleep(1000);
            printf("%s sells ticket:%d\n",td->name.c_str(),ticket--);
            // pthread_mutex_unlock(td->plock);
        }
        else
        {
            // pthread_mutex_unlock(td->plock);
            break;
        }
    }
    return nullptr;
}

int main()
{
    pthread_t t1,t2,t3,t4;
    pthread_mutex_t lock;
    pthread_mutex_init(&lock,nullptr);

    threaddata_t data1 = {"thread-1",&lock};
    pthread_create(&t1,nullptr,route,(void*)&data1);

    threaddata_t data2 = {"thread-2", &lock};
    pthread_create(&t2, nullptr, route, (void*)&data2);

    threaddata_t data3 = {"thread-3", &lock};
    pthread_create(&t3, nullptr, route, (void*)&data3);

    threaddata_t data4 = {"thread-4", &lock};
    pthread_create(&t4, nullptr, route, (void*)&data4);

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    pthread_join(t4, nullptr);

    pthread_mutex_destroy(&lock);

    return 0;
}


// int tickets = 5000;
// pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;

// uint64_t getMicrosecondTimestamp()
// {
//     auto now = std::chrono::system_clock::now();
//     auto microseconds = std::chrono::time_point_cast<std::chrono::microseconds>(now);
//     auto epoch = microseconds.time_since_epoch();
//     return static_cast<uint64_t>(epoch.count());
// }

// void route()
// {
//     char name[64];
//     pthread_getname_np(pthread_self(),name,sizeof(name));

//     uint64_t start = getMicrosecondTimestamp();

//     while(1)
//     {
//         // 加锁
//         pthread_mutex_lock(&glock);
//         if(tickets>0)
//         {
//             usleep(1000);
//             printf("%s sells ticket:%d\n",name,tickets);
//             tickets--;

//             // 解锁
//             pthread_mutex_unlock(&glock);
//         }
//         else
//         {
//             pthread_mutex_unlock(&glock);
//             break;
//         }
//     }
//     uint64_t end = getMicrosecondTimestamp();
//     std::cout<<name<<" cast: "<<end - start<<std::endl;
// }

// int main()
// {
//     std::vector<Thread> threads;
//     for(int i = 0; i < 4; i++)
//     {
//         threads.emplace_back(route);
//     }

//     for(auto& thread:threads)
//     {
//         thread.start();
//     }

//     for(auto& thread:threads)
//     {
//         thread.join();
//     }

//     return 0;
// }