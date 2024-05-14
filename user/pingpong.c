#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char* argv[])
{
	if(argc != 1)
	{
		printf("fail");
		exit(1);
	}
	int p1[2], p2[2];
	char* buf[1];
	pipe(p1);
	pipe(p2);
	if(fork() > 0)
	{
		write(p1[1], "i", 1);
		read(p2[0], buf, 1);
		printf("%d: received pong\n", getpid());
	}
	else
	{
		read(p1[0], buf, 1);
		printf("%d: received ping\n", getpid());
		write(p2[1], "o", 1);
	}

	exit(0);
}
