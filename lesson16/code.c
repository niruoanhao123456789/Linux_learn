#include<stdio.h>
#include<unistd.h>

struct obj
{
	int a;
	int b;
	char c;
	double d;
};

int main()
{
	struct obj tmp;
	printf("&tmp: %p, &(tmp.a): %p\n", &tmp, &(tmp.a));
	long long offset = (long long)&(((struct obj*)0)->d);
	
	long long start = (long long)&tmp.d - offset;
	printf("offset: %lld\n", offset);

	printf("Address对比: %p, %p\n", &tmp, start);

	return 0;
}
