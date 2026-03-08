#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int flag = 1;

int main()
{
	pid_t id = fork();
	if(id == 0)
	{
       	 // 子进程
        	while(1)
		{
           		 printf("子进程：\npid: %d, ppid: %d, flag: %d, &flag: %p\n", getpid(), getppid(), flag, &flag);
			 flag++;
            sleep(1);
        }
    }
    else if(id > 0)
    {
        // 父进程
        while(1)
        {
            printf("父进程：\npid: %d, ppid: %d, flag: %d, &flag: %p\n", getpid(), getppid(), flag, &flag);
	    sleep(1);
        }
    }
    else{}

	//char pwd[128] = {0};
	//getcwd(pwd,128);
	//printf("This is current cwd:%s before chdir\n",pwd);
	//chdir("/home/yjm/118/lesson14");
	//getcwd(pwd,sizeof(pwd));
	//printf("This is current cwd:%s after chdir\n",pwd);

	//int i = 3;
	//while(i)
	//{
	//	printf("This is a process, its pid:%d ppid:%d\n",getpid(),getppid());
	//	sleep(1);
	//}

	//return 0;
}
