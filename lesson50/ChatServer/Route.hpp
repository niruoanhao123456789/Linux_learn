#pragma once
#include<iostream>
#include<vector>
#include<string>
#include"InetAddr.hpp"
#include"Mutex.hpp"
#include"Logger.hpp"
using namespace LogModule;

class Route
{
private:
    bool IsOnline(const InetAddr &who)
    {
        for (auto &user : _users)
        {
            if (user == who)
                return true;
        }
        return false;
    }
    void AddUser(const InetAddr &who)
    {
        _users.push_back(who);
    }

public:
    Route() {}
    void RouteMessage(std::string message, InetAddr who, int sockfd)
    {
        LOG(LogLevel::DEBUG) << "3. RouteMessage";
        {
            LockGuard lockguard(&_lock);
            // 1. 判定用户是否已经在线: 不在就插入表示上线 ， 如果在，就什么都不管，直接转发
            if (!IsOnline(who))
            {
                AddUser(who);
            }

            // 2. 转发
            for (auto &user : _users)
            {
                std::string send_message = who.StringAddress();
                send_message += "# ";
                send_message += message;
                sendto(sockfd, send_message.c_str(), send_message.size(), 0, user.Addr(), user.AddrLen());
            }
        };
    }

private:
    std::vector<InetAddr> _users;
    Mutex _lock;
};