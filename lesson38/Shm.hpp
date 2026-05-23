#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<sys/shm.h>
#include <signal.h>

const std::string project_name = "/home";
const int project_id = 0x6666;
const int g_size = 4096;

static std::string ToHex(long long data)
{
    char hex[64] = {0};
    snprintf(hex,sizeof(hex),"0x%lld",data);
    return hex;
}

typedef struct Data
{
    int _count;
    char _buffer[26*2];
}buffer_t;

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

    // 1、创建
    bool Create()
    {
        return CreateCoreHelper(IPC_CREAT|IPC_EXCL|0666);
    }

    // 2、获取共享内存
    bool GetShm()
    {
        return CreateCoreHelper(IPC_CREAT);
    }

    // 3、删除共享内存
    bool DeleteShm()
    {
        int n = shmctl(_shmid,IPC_RMID,nullptr);
        return n<0?false:true;
    }

    // 4、获取共享内存属性
    void GetShmAttr()
    {
        struct shmid_ds ds;
        int n = shmctl(_shmid,IPC_STAT,&ds);
        if(n<0)
        {
            perror("shmctl");
            return;
        }
        std::cout<<"pid: "<<getpid()<<std::endl;
        std::cout<<ds.shm_cpid<<std::endl;
        std::cout<<ds.shm_segsz<<std::endl;
        std::cout<<ToHex(ds.shm_perm.__key)<<std::endl;
    }

    void* Attach()
    {
        _start = shmat(_shmid,nullptr,0);
        return _start;
    }

    void Detach()
    {
        shmdt(_start);
    }

    void Debug()
    {
        std::cout<<"Key: "<<ToHex(_key)<<std::endl;
        std::cout<<"shmid: "<<_shmid<<std::endl;
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

    bool CreateCoreHelper(int flags)
    {
        // 1、获取key值
        key_t key = GetKey();
        // 2、创建共享内存
        _shmid = shmget(key,_size,flags);
        if(_shmid<0)
        {
            perror("shmget");
            return false;
        }
        return true;
    }

    key_t _key;
    int _shmid;
    int _size;
    void* _start;
};