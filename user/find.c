#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void find(char* path, char* file_name)
{
	char buf[512];
	char *p;
	int fd;
	struct dirent de;
	struct stat st;
	
	if((fd = open(path, 0)) < 0)
	{
		printf("open false\n");
		return;
	}
	if(fstat(fd, &st) < 0)
	{
		printf("get status false\n");
		return;
	}

	if(st.type != T_DIR)
	{
		printf("not dir\n");
		return;
	}

	if((strlen(path) + 1 + DIRSIZ + 1) > sizeof buf)
	{
		printf("path too long\n");
		return;
	}
	strcpy(buf, path);
	p = buf + strlen(buf);
	*p++ = '/';
	
	while(read(fd, &de, sizeof(de)) == sizeof(de))
	{
		if(de.inum == 0)
		{
			continue;
		}
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		if(stat(buf, &st) < 0)
		{
			printf("find: can\'t stat %s\n", buf);
			continue;
		}
		if(st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0)
		{
			find(buf, file_name);
		}
		else if(strcmp(file_name, p) == 0)
		{
			printf("%s\n", buf);
		}
	}
	close(fd);
}

void main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("usage: find <path> <filename>\n");
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(0);
}
