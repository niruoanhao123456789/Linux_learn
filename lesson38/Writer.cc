#include"Shm.hpp"

Shm shm;

class Init
{
public:
    Init()
    {
        shm.GetShm();
        _addr = (char*)shm.Attach();
        std::cout<<"addr: "<<ToHex((long long)_addr)<<std::endl;
    }

    ~Init()
    {
        shm.Detach();
    }

    char* Addr()
    {
        return _addr;
    }

    char* _addr;
};

Init init;

int main()
{
    std::cout<<"test begin..."<<std::endl;
    buffer_t* shm = (buffer_t*)init.Addr();
    shm->_count = 0;
    memset(shm->_buffer,0,4096);

    char ch = 'A';
    for(int i=0;i<2*26;i+=2,ch++)
    {
        shm->_buffer[i]=ch;
        usleep(234219);
        shm->_buffer[i+1] = ch;
        usleep(734217);
        // ?
        shm->_count++;
        usleep(734214);

        sleep(1);
    }

    return 0;
}