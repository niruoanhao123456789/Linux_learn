#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<cstdio>
#include<unistd.h>
#include<sys/shm.h>
#include <signal.h>

const std::string project_name = "/home";
const int project_id = 0x6666;
const int g_size = 4096;

class Shm
{
public:
    Shm(int size = g_size)
    : _key(0)
    , _shmid(-1)
    , _size(size)
    , _start(nullptr)
    {
    }

private:
    key_t GetKey()
    {
        _key = ftok(project_name.c_str(),project_id);
        if(_key<0)
        {
            perror("ftok");
        }
        return _key;
    }

    key_t _key;
    int _shmid;
    int _size;
    void* _start;
};