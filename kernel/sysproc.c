#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

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
  //
  if(n > 0)
  {
    myproc()->sz = myproc()->sz + n;
  }
  else if(myproc()->sz + n > 0)
  {
    myproc()->sz = uvmdealloc(myproc()->pagetable, myproc()->sz, myproc()->sz + n);
  }
  else
  {
    return -1;
  }
  
  /*
  if(growproc(n) < 0)
    return -1;
  */
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
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
  backtrace();
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

uint64
sys_sigalarm(void)
{
  int tickss;
  uint64 handlers;

  argint(0, &tickss);
  argaddr(1, &handlers);

  struct proc *p = myproc();
  p->ticks = tickss;
  p->handler = handlers;
  p->tick_count = 0;

  return 0;
}

uint64
sys_sigreturn(void)
{
  struct proc *p = myproc();

  p->is_work_alarm = 0;
  
  p->trapframe->kernel_satp = p->t_temp->kernel_satp;
  p->trapframe->kernel_sp = p->t_temp->kernel_sp;
  p->trapframe->kernel_trap = p->t_temp->kernel_trap;
  p->trapframe->epc = p->t_temp->epc;
  p->trapframe->kernel_hartid = p->t_temp->kernel_hartid;
  p->trapframe->ra = p->t_temp->ra;
  p->trapframe->sp = p->t_temp->sp;
  p->trapframe->gp = p->t_temp->gp;
  p->trapframe->tp = p->t_temp->tp;
  p->trapframe->t0 = p->t_temp->t0;
  p->trapframe->t1 = p->t_temp->t1;
  p->trapframe->t2 = p->t_temp->t2;
  p->trapframe->s0 = p->t_temp->s0;
  p->trapframe->s1 = p->t_temp->s1;
  p->trapframe->a0 = p->t_temp->a0;
  p->trapframe->a1 = p->t_temp->a1;
  p->trapframe->a2 = p->t_temp->a2;
  p->trapframe->a3 = p->t_temp->a3;
  p->trapframe->a4 = p->t_temp->a4;
  p->trapframe->a5 = p->t_temp->a5;
  p->trapframe->a6 = p->t_temp->a6;
  p->trapframe->a7 = p->t_temp->a7;
  p->trapframe->s2 = p->t_temp->s2;
  p->trapframe->s3 = p->t_temp->s3;
  p->trapframe->s4 = p->t_temp->s4;
  p->trapframe->s5 = p->t_temp->s5;
  p->trapframe->s6 = p->t_temp->s6;
  p->trapframe->s7 = p->t_temp->s7;
  p->trapframe->s8 = p->t_temp->s8;
  p->trapframe->s9 = p->t_temp->s9;
  p->trapframe->s10 = p->t_temp->s10;
  p->trapframe->s11 = p->t_temp->s11;
  p->trapframe->t3 = p->t_temp->t3;
  p->trapframe->t4 = p->t_temp->t4;
  p->trapframe->t5 = p->t_temp->t5;
  p->trapframe->t6 = p->t_temp->t6;

  return 0;
}
