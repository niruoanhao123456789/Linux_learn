#include "BlockQueue_v2.hpp"
#include "Task.hpp"
#include <pthread.h>
#include <unistd.h>

void *Consumer(void *args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    while(true)
    {
        Task t;
        // 1. 消费任务
        bq->Pop(&t);

        // 2. 处理任务
        t();
        std::cout << "消费了数据: " << t.tostring() << t.result() << std::endl;
    }
}

void *Productor(void *args)
{
    BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(args);
    int data = 10;
    while(true)
    {   
        sleep(1);
        // 1. 构建任务,网络模块，其他模块？？
        Task t(data, data*10);

        // 2. 生产任务
        bq->Push(t);
        std::cout << "生产: " << t.tostring() << "?" << std::endl;
        data++;
    }
}

// 单生产，单消费
// 多生产，多消费
int main()
{
    // 1. 构建交易场所
    BlockQueue<Task> *bq = new BlockQueue<Task>();

    pthread_t c, p;
    // 2. 创建两种角色
    pthread_create(&c, nullptr, Consumer, (void *)bq);
    pthread_create(&p, nullptr, Productor, (void *)bq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    delete bq;

    return 0;
}