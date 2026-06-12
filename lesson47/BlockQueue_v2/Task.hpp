#pragma once
#include<iostream>
#include<string>

class Task
{
public:
    Task(int a=0,int b=0)
    :_x(a)
    ,_y(b)
    ,_res(0)
    {}

    void operator()()
    {
        _res = _x +_y;
    }

    std::string tostring()
    {
        return std::to_string(_x) + "+" + std::to_string(_y) + "=";
    }

    int result()
    {
        return _res;
    }

private:
    int _x;
    int _y;
    int _res;
};