#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
    umask(0);
    close(1);
    int fd = open("log.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    // stdout -> 1
    printf("fd: %d\n", fd); // stdout->缓冲区里面->close(fd)-> 关闭刷新通道，数据无法送用户到内核
    fflush(stdout);
    close(fd); // 系统调用
    return 0;
}


// #define ONE (1<<0)   // 1
// #define TWO (1<<1)   // 1
// #define THREE (1<<2) // 4
// #define FOUR (1<<3)  // 8
// #define FIVE (1<<4)  // 16

// void Print(int flags)
// {
//    if(flags & ONE)
//        printf("ONE\n");
//    if(flags & TWO)
//        printf("TWO\n");
//    if(flags & THREE)
//        printf("THREE\n");
//    if(flags & FOUR)
//        printf("FOUR\n");
//    if(flags & FIVE)
//        printf("FIVE\n");
// }

// int main()
// {
//    Print(ONE);
//    printf("\n");
//    Print(TWO);
//    printf("\n");
//    Print(ONE | TWO);
//    printf("\n");
//    Print(ONE | TWO | THREE);
//    printf("\n");
//    Print(ONE | TWO | THREE | FOUR);
//    printf("\n");
//    Print(TWO | THREE | FOUR | FIVE);

//    return 0;
// }