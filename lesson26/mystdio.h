#pragma once
#include<string.h>
#include<sys/stat.h>
//#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<assert.h>

typedef struct 
{
    int _fd;
    int _flags;
    int _mode;
    int _size;
    int _capacity;
    char outbuffer[1024];
    //char inbuffer[1024];
}My_File;

#define NONE_CACHE 1
#define LINE_CACHE 2
#define FULL_CACHE 4

My_File* Myfopen(const char* pathname,const char* mode); // r w a
int Myfwrite(const char* message,int size,int num,My_File* fp);
void Myfclose(My_File* fp);
void Myfflush(My_File* fp);