#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64 getFreeMemory();
uint64 getNumberProcesses();
uint64 getAllNumberProcesses();

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_trace(void)
{
//mask
  int n;
  argint(0, &n);
//printf("this is sys_trace, mask is %d\n", n);

//input mask to process
  struct proc *p = myproc();
  p->trace_mask = n;

  return 0;
}

uint64 sys_sysinfo(void)
{
  struct sysinfo info;
  uint64 st;
  struct proc *p = myproc();

  info.nproc = getNumberProcesses();
  info.freemem = getFreeMemory();//pages num, not size
  info.allproc = getAllNumberProcesses();
  
  argaddr(0, &st);

  if(copyout(p->pagetable, st, (char*)&info, sizeof(info)) < 0)
  {
    return -1;
  }

  printf("proc num is %d\nfree pages is %d and size is %d\nall proc is %d, arve is %d\%\n", info.nproc, info.freemem, (info.freemem)*4096, info.allproc, (info.nproc * 100 / info.allproc));
  
  return 0;
}

