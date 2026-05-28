#include<iostream>
#include<thread>
#include<unistd.h>
#include<pthread.h>

// void hello()
// {
//     while(1)
//     {
//         std::cout<<"我是线程..., pid: "<<getpid()<<std::endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     std::thread t(hello);
//     while(1)
//     {
//         std::cout<<"我是主线程..., pid: "<<getpid()<<std::endl;
//         sleep(1);
//     }
//     t.join();

//     return 0;
// }


void *hello(void *args)
{
    const char *name = (const char*)args;
    while(true)
    {
        std::cout << "我是新线程..., pid: " << getpid() << " name is : " << name << std::endl;
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid,nullptr,hello,(void*)"new-thread");
    
    while(1)
    {
        std::cout<<"我是主线程..., pid: "<<getpid()<<std::endl;
        sleep(5);
    }

    return 0;
}