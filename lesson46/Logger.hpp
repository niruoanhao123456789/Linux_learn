#pragma once
#include<iostream>
#include<cstdio>
#include<string>
#include<ctime>
#include<filesystem>
#include<fstream>
#include<sstream>
#include<memory>
#include<unistd.h>

namespace LogModule
{
    // 1、获取时间戳
    std::string GetTimeStamp()
    {
        tmie_t timestamp = time(nullptr);
        struct tm data_time;
        localtime_r(&timestamp,&data_time);

        char data_time_str[128];
        snprintf(data_time_str,sizeof(data_time_str),"%4d-%02d-%02d-%02d:%02d:%02d",
                data_time.tm_year + 1900,
                data_time.tm_mon + 1,
                data_tmie.tm_mday,
                data_time.tm_hour,
                data_time.tm_min,
                data_time.tm_sec);
        
        return data_time_str;
    }

    // 2、日志等级
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    std::string LogLevelToString(LogLevel level)
    {
        switch(level)
        {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::EEROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOW";
        }
    }

    // 3、日志刷新
    // 基类：策略基类  根据不同策略设置刷新策略的派生类
    class LogStrategy
    {
    public:
        virtual ~LogStrategy() = default;
        virtual void SyncLog(const std::string& logmessage) = 0;
    };

    
}