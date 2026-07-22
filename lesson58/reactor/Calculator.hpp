#pragma once
#include<iostream>
#include<string>
#include"Protocol.hpp"

class Request;
class Response;

class Calculator
{
public:
    Response Exec(const Request& req)
    {
        LOG(LogLevel::DEBUG)<<"Enter Calculator Exec";
        Response resp; // {0,0}
        switch(req._oper)
        {
        case '+':
            resp._result = req._x + req._y;
            break;
        case '-':
            resp._result = req._x - req._y;
            break;
        case '*':
            resp._result = req._x * req._y;
            break;
        case '/':
            if(req._y==0)
                resp._exitcode = 1;
            else
                resp._result = req._x / req._y;
            break;
        case '%':
            if(req._y==0)
                resp._exitcode = 2;
            else
                resp._result = req._x % req._y;
            break;
        default:
            resp._exitcode = 3;
            break;
        }

        return resp;
    }
};