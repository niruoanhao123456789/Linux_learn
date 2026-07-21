#pragma once

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include "Logger.hpp"

using namespace LogModule;

class ExecuteCommand
{
public:
    ExecuteCommand()
    {
        // _black_list.push_back("rm"); // ls -al && rm * -rf
        // _black_list.push_back("unlink");
        // _black_list.push_back("mkdir");
        // _black_list.push_back("cp");

        _white_list.push_back("pwd");
        _white_list.push_back("who");
        _white_list.push_back("whoami");
        _white_list.push_back("ls -a -l");
        _white_list.push_back("env");
    }
    ~ExecuteCommand()
    {}
private:
    // 安全：true
    // 否则: false
    bool IsSafe(const std::string &cmdstr)
    {
        for(auto &cmd: _white_list)
        {
            if(cmd == cmdstr)
                return true;
        }

        return false;
    }
public:
    // "ls -a -l" -> 结果
    std::string Exec(std::string cmdstr)
    {
        if(!IsSafe(cmdstr))
        {
            return "UnSafe";
        }
        FILE *fp = popen(cmdstr.c_str(), "r");
        if(fp == nullptr)
        {
            LOG(LogLevel::ERROR) << "exec error: " << cmdstr;
            return "error";
        }
        std::string result;
        char buffer[512];
        while(fgets(buffer, sizeof(buffer), fp) != nullptr)
        {
            result += buffer;
            buffer[0] = 0;
        }

        pclose(fp);
        return result;
    }
private:
    std::vector<std::string> _white_list;
};