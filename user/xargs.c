#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define GETINPUT 16


void main(int argc, char* argv[])
{
    //wait for cmd1 result
    sleep(42);
    //get putout from cmd
    char buf[GETINPUT];
    read(0, buf, GETINPUT);
    printf("get stand input is: %s", buf);

    //get cmd2
    char* myArgv[MAXARG];
    int myArgc = 0;
    for(int i = 1; i < argc; i++)
    {
        myArgv[myArgc] = argv[i];
        myArgc++;
    }
    //excute cmd2 by exec
    char *p = buf;
    for(int i = 0; i < GETINPUT; i++)
    {
        if(buf[i] == '\n')
        {
            if(fork() > 0)
            {
                p = &buf[i + 1];
                wait(0);
            }
            else
            {
                buf[i] = 0;
                myArgv[myArgc] = p;
                myArgc++;
                myArgv[myArgc] = 0;//cmd[] end 
                myArgc++;
                exec(myArgv[0], myArgv);
                exit(0);
            }
        }
    }
    wait(0);
    exit(0);
}
