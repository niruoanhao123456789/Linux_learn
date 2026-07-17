#pragma once
#include<iostream>
#include<string>
#include<cstdio>
#include<vector>
#include"Logger.hpp"
using namespace LogModule;

class ExecuteCommand
{
public:
    ExecuteCommand()
    {
        _white_list.push_back("pwd");
        _white_list.push_back("who");
        _white_list.push_back("whoami");
        _white_list.push_back("ls -a -l");
        _white_list.push_back("env");
    }

    // "ls -a -l" -> 结果
    std::string Exec(std::string& cmdstr)
    {
        if(!IsSafe(cmdstr))
            return "Unsafe";
        FILE* fp = popen(cmdstr.c_str(),"r");
        if(!fp)
        {
            LOG(LogLevel::ERROR)<<"exec error: "<<cmdstr;
            return "error";
        }
        std::string result;
        char buffer[512]={0};
        while(fgets(buffer,sizeof(buffer),fp)!=nullptr)
        {
            result+=buffer;
            buffer[0]=0;
        }

        pclose(fp);
        fp = nullptr;
        return result;
    }

private:
    bool IsSafe(const std::string& cmdstr)
    {
        for(auto& cmd: _white_list)
        {
            if(cmd==cmdstr)
                return true;
        }
        return false;
    }

    std::vector<std::string> _white_list;
};