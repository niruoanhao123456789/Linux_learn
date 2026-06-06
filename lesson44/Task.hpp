#pragma once
#include<iostream>
#include<string>
#include<unistd.h>

class Task
{
public:
    Task(const std::string& who,int left,int right)
    :_left(left)
    ,_right(right)
    ,_result(0)
    {}

    void Execute()
    {
        _result = _left+_right;
    }

    std::string Result()
    {
        return std::to_string(_left)+"+"+std::to_string(_right)+"="+std::to_string(_result);
    }

private:
    int _left;
    int _right;
    int _result;
    std::string _who;
};