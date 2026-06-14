#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include "Mutex.hpp"

int tickets = 1000;
Mutex lock;
pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gcond = PTHREAD_COND_INITIALIZER;

void buyTicket(int id)
{
    while (1)
    {
        // 临界区
        //LockGuard lockguard(&lock); // RAII风格的加锁逻辑

        pthread_mutex_lock(&glock);
        pthread_cond_wait(&gcond, &glock);

        if (tickets > 0)
        {
            usleep(1000);
            std::cout << "Thread " << id << " bought ticket, remaining: " << --tickets << std::endl;
            pthread_mutex_unlock(&glock);
        }
        else
        {
            pthread_mutex_unlock(&glock);
            break;
        }
    }
}

int main()
{
    std::thread threads[4];
    for (int i = 0; i < 4; i++)
    {
        threads[i] = std::thread(buyTicket, i + 1);
    }

    sleep(3);

    while (1)
    {
        std::cout << "wake up ..." << std::endl;
        // pthread_cond_signal(&gcond);
        pthread_cond_broadcast(&gcond);
        sleep(1);
    }


    for(auto& thread:threads)
    {
        thread.join();
    }
    std::cout<<"All tickets were sold out!"<<std::endl;

    return 0;
}