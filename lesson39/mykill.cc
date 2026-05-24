#include<iostream>
#include<string>
#include<signal.h>
#include<unistd.h>

void Usage(std::string proc)
{
    std::cerr<<"Usage:\n\t"<<proc<<"signumber pid\n\n";
}

// ./mykill signumber pid
// ./mykill 9 1234
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        Usage(argv[0]);
        return 1;
    }
    int signumber = std::stoi(argv[1]);
    pid_t pid = std::stoi(argv[2]);
    
    std::cout<<"send "<<signumber<<"to "<<pid<<std::endl;
    
    kill(pid,signumber);

    return 0;
}