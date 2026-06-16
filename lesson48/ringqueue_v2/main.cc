#include <iostream>
#include <memory>
#include <unistd.h>
#include <functional>
#include "ringqueue_v2.hpp"
#include "Thread.hpp"

using task_t = std::function<void()>;

void Task()
{
    char name[64];
    pthread_getname_np(pthread_self(), name, sizeof(name));
    std::cout << "我是一个任务, 处理我的是: " << name << std::endl;
}


int main()
{
    std::unique_ptr<RingQueue<task_t> > ringqueue = std::make_unique<RingQueue<task_t> >();

    // 多生产者，多消费者？？

    auto consumer_cb = [&ringqueue](){
        while (true)
        {
            // 1. 消费任务 && 数据
            task_t t;
            ringqueue->Pop(&t);

            // 2. 处理任务
            t();
            // std::cout << "消费数据 : " << data << std::endl;
        }
        
    };

    auto productor_cb = [&ringqueue](){
        // int data = 0;
        while(true)
        {
            sleep(1);
            // 1. 获取任务&&数据

            // 2. 生产
            ringqueue->Push(Task);
            //std::cout << "生产任务" << std::endl;
        }
    };

    Thread c1(consumer_cb);
    Thread c2(consumer_cb);
    Thread c3(consumer_cb);
    Thread p1(productor_cb);
    Thread p2(productor_cb);


    c1.start();
    c2.start();
    c3.start();
    p1.start();
    p2.start();


    c1.join();
    c2.join();
    c3.join();
    p1.join();
    p2.join();

    return 0;
}