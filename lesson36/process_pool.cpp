#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<memory>
#include<functional>
#include<unistd.h>
#include<sys/wait.h>

// 先描述
class Channel
{
public:
    Channel(int wfd,pid_t who)
    :_wfd(wfd)
    ,_sub_process_id(who)
    {
        _name = "Channel-" + std::to_string(_sub_process_id) + std::to_string(_wfd);
    }

    int fd()
    {
        return _wfd;
    }

    pid_t sub_id()
    {
        return _sub_process_id;
    }

    std::string name()
    {
        return _name;
    }

    void Close()
    {
        if(_wfd>=0)
            close(_wfd);
    }

    void Wait()
    {
        pid_t rid = waitpid(_sub_process_id,nullptr,0);
    }

    

private:
    int _wfd;
    pid_t _sub_process_id;
    std::string _name; 
}