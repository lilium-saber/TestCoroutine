#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include "kernel/sysinfo.h"



void main(int argc, char* argv[])
{
    struct sysinfo info;
    if(argc != 1)
    {
        printf("error sysinfo!\n");
    }
    printf("sysinfo work\n");

    if(sysinfo(&info) < 0)
    {
        printf("failed sysinfo\n");
    }

    exit(0);
}
