#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
	int time = 0;
	if(argc < 2)
	{
		printf("fail");
		exit(1);
	}
	time = atoi(argv[1]);
	sleep(time);
	printf("sleep succes!");
	exit(0);

}
