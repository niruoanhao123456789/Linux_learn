#include"mystdio.h"

int main()
{
    My_File* fp = Myfopen("log.txt", "a");
    if(!fp)
        return 1;

    int cnt = 3;
    const char* msg = "hello world ";
    while(cnt--)
    {
        Myfwrite(msg, 1, strlen(msg), fp);
        Myfflush(fp);
        sleep(1);
    }

    Myfclose(fp);
    return 0;
}