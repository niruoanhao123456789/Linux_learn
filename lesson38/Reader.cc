#include"Shm.hpp"

// Writer -> shm -> Reader
int main()
{
    // 1在内核中创建共享内存
    Shm shm;
    shm.Create();
    sleep(100);
    char* addr = (char*)shm.Attach();

    buffer_t* shm_addr = (buffer_t*)addr;
    int old = shm_addr->_count;
    while(1)
    {
        if(old!=shm_addr->_count)
        {
            std::cout << "count : " << shm_addr->_count << std::endl;
            std::cout << "data  : " << shm_addr->_buffer << std::endl;
            old = shm_addr->_count;
        }

        usleep(74329);
        if (shm_addr->_count >= 26)
             break;
    }

    shm.Detach();
    shm.DeleteShm();

    return 0;
}