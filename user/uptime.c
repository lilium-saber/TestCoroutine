#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void main(int argc, char* argv[])
{
	if(argc != 1)
	{
		printf("only uptime\n");
		exit(1);
	}
	printf("start from 0, print 10 time\n");
	for(int i = 0; i < 10; i++)
	{
		sleep(1);
		printf("%d\n", i + 1);
	}
	exit(0);
}
