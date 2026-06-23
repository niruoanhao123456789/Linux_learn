#include "Task.hpp"
#include "ThreadPool.hpp"
#include <memory>

int main()
{
    ENABLE_CONSOLE_LOG_STRATEGY();

    LOG(LogLevel::DEBUG) << "进程已经跑了很久了";
    sleep(3);

    // std::unique_ptr<ThreadPool<task_t>> tp = std::make_unique<ThreadPool<task_t>>();
    // 创建单例的过程，有没有可能多线程创建呢？？
    ThreadPool<task_t>::GetInstance()->Start();

    int count = 10;
    while (count--)
    {
        LOG(LogLevel::DEBUG) << "-------------------------: " << count;
        sleep(1);
        ThreadPool<task_t>::GetInstance()->Push(task1);
        
        sleep(1);
        ThreadPool<task_t>::GetInstance()->Push(task2);
    }

    ThreadPool<task_t>::GetInstance()->Stop();
    ThreadPool<task_t>::GetInstance()->Wait();

    return 0;
}