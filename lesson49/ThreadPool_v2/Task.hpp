#pragma once
#include <iostream>
#include <functional>
#include <pthread.h>
#include "Logger.hpp"

using task_t = std::function<void()>;

using namespace LogModule;

// 1. 全局函数
void task1()
{
    char name[64];
    pthread_getname_np(pthread_self(), name, sizeof(name));
    LOG(LogLevel::DEBUG) << "执行任务1: 打印消息 |" << name << "|";
}

void task2()
{
    char name[64];
    pthread_getname_np(pthread_self(), name, sizeof(name));
    LOG(LogLevel::DEBUG) << "执行任务2: 计算 1+1 = " << 1 + 1 << " |" << name << "|";
}