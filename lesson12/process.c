#include"process.h"

#define SIZE 53
#define LABLE '='

void Process(double total, double current, double speed, const char *userinfo)
{
	if(current>total)
		return;

	const char* lable[]={"loading.","loading..","loading..."};
	int size = sizeof(lable)/sizeof(lable[0]);
	static int index = 0;
	// 绝对不能一次循环就把整个进度推进完毕
	// 计算比率
	double rate = current*100/total;

	char out_bar[SIZE]={0};
	memset(out_bar,'\0',sizeof(out_bar));
	// 填充进度字符
	for(int cnt = 0;cnt<(int)rate/2;cnt++)
	{
		out_bar[cnt]=LABLE;
	}
	// 刷新进度条
	printf("[%-50s][%4.1lf%%][%-10s] | %6.1lf/%6.1lf,speed: %.1lf%s\r",out_bar, rate, lable[index], current, total, speed, userinfo);
	fflush(stdout);
	index++;
	index %= size;
	// 进度完成换行
	if(current>=total)
	{
		printf("\n");
	}
}
