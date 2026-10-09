#include <iostream>
#include <sys/mman.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

// 使用mmap来分配内存
void* my_malloc(size_t size)
{
    void* ptr = mmap(nullptr,size,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
    if(ptr == MAP_FAILED)
    {
        exit(EXIT_FAILURE);
    }

    return ptr;
}

// 使用munmap来释放内存
void my_free(void* ptr, size_t size)
{
    if(munmap(ptr,size)==-1)
    {
        exit(EXIT_FAILURE);
    }
}

int main()
{
    size_t size = 1024; // 分配1KB内存
    char* ptr = (char*)my_malloc(size);

    // 使用分配的内存（这里只是简单地打印指针值）
    printf("Allocated memory at address: %p\n", ptr);

    // ... 在这里可以使用ptr指向的内存 ...
    memset(ptr, 'A', size);

    for(int i = 0; i < size; i++)
    {
        printf("%c ", ptr[i]);
        sleep(1);
    }

    // 释放分配的内存
    my_free(ptr, size);

    return 0;
}