#include"mystdio.h"

static mode_t gmode = 0666;

My_File* Myfopen(const char* pathname,const char* mode)
{
    if(pathname == NULL || mode == NULL)
    {
        return NULL;
    }
    umask(0);
    int fd = 0;
    int flags = 0;
    if(strcmp(mode,"w")==0)
    {
        flags = O_CREAT|O_WRONLY|O_TRUNC;
        fd = open(pathname,flags,gmode);
    }
    if(strcmp(mode,"r")==0)
    {
        flags = O_RDONLY;
        fd = open(pathname,flags);
    }
    if(strcmp(mode,"a")==0)
    {
        flags = O_CREAT|O_WRONLY|O_APPEND;
        fd = open(pathname,flags,gmode);
    }

    if(fd<0)
    {
        return NULL;
    }

    // 创建My_File对象
    My_File* fp = (My_File*)malloc(sizeof(My_File));
    if(!fp)
        return NULL;

    fp->_fd = fd;
    fp->_flags = flags;
    fp->_mode = LINE_CACHE;
    fp->_capacity = 1024;
    fp->_size = 0;
    memset(fp->outbuffer,0,sizeof(fp->outbuffer));
    return fp;
}

void Myfflush(My_File* fp)
{
    if(!fp)
        return;

    // 判断是否刷新
    if(fp->_size>0)
    {
        // 系统调用
        // 将用户缓冲区拷贝到内核，WB，Write Back
        write(fp->_fd,fp->outbuffer,fp->_size);
        fp->_size = 0;
        // WT,Write Through
        // 不仅仅要写入到内核缓冲区，必须给我写到对应的硬件上
        fsync(fp->_fd);
    }
}

int Myfwrite(const char* message,int size,int num,My_File* fp)
{
    if(message==NULL||fp==NULL)
        return -1;
    // 向C文件里面写，本质是向缓冲区写
    int total = size*num;
    if(total + fp->_size > fp->_capacity-1)
        return -1;
    
    // 写入
    memcpy(fp->outbuffer+fp->_size,message,total);
    fp->_size += total;
    fp->outbuffer[fp->_size]=0;

    if(fp->outbuffer[fp->_size-1]=='\n'&&(fp->_mode & LINE_CACHE))
        Myfflush(fp);

    return num;
}

void Myfclose(My_File* fp)
{
    if(!fp)
        return;

    Myfflush(fp);
    close(fp->_fd);
}