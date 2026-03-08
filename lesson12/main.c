#include"process.h"

// 有一个下载任务
double gtotal = 1024.0; // 目标文件的总大小
double gspeed = 1.0; // 网络速度

void DownLoad(double total, flush_t cb)
{
	double current = 0; // 当前下载量
	while(1)
	{
		usleep(5000);
		current += gspeed;
		if(current>=gtotal)
		{
			current = gtotal;
			cb(gtotal,current,gspeed,"MB/s");
			break;
		
		}
		else
		{	
			cb(gtotal,current,gspeed,"MB/s");
		}
	
	}
}

int main()
{
	DownLoad(gtotal,Process);

	return 0;
}
