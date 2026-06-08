#include<iostream>
#include<unistd.h>
#include<vector>
#include"Thread.hpp"

void hello()
{
    char name[64];
    pthread_getname_np(pthread_self(),name,sizeof(name));

    int cnt = 5;
    while(cnt--)
    {
        std::cout<<"hello thread "<<name<<std::endl;
        sleep(1);
    }
}

int main()
{
    std::vector<Thread> threads;
    for(int i=0;i<10;i++)
    {
        threads.emplace_back(hello);
    }

    for(auto &thread : threads)
        thread.start();

    // for(auto &thread : threads)
    //     thread.detach();

    // for(auto &thread : threads)
    //     thread.start();

    for(auto &thread : threads)
        thread.join();


    sleep(3);

    return 0;
}