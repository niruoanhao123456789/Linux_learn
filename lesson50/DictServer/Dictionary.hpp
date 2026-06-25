#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "Logger.hpp"

std::string defaultdict = "./Dict.txt";
std::string gsep = ": ";

using namespace LogModule;

// 就是一个字典，英文翻译成为中文的类
class Dictionary
{
private:
    void LoadConf()
    {
        std::ifstream in(_dictfilename);
        if (!in.is_open())
        {
            LOG(LogLevel::FATAL) << "open " << _dictfilename << " error";
            exit(1);
        }
        std::string line;
        while (std::getline(in, line))
        {
            // apple: 苹果 - I eat an apple every day. / 我每天吃一个苹果。
            // [)
            auto pos = line.find(gsep);
            if (pos == std::string::npos)
            {
                LOG(LogLevel::WARNING) << "load error: " << line;
                continue;
            }
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + gsep.size());
            _dictmap.insert({key, value});
        }

        in.close();
    }

public:
    Dictionary(const std::string &dictfilename = defaultdict)
        : _dictfilename(dictfilename)
    {
        LoadConf();
    }
    std::string Translate(const std::string &word)
    {
        auto iter = _dictmap.find(word);
        if(iter == _dictmap.end())
        {
            return "未知";
        }
        return iter->second;
    }
    ~Dictionary()
    {
    }

private:
    std::string _dictfilename;
    std::unordered_map<std::string, std::string> _dictmap;
};