#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

extern int trace_enabled;
extern int syscall_counts[26];

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_trace(void)
{
  int enable;

  if(argint(0, &enable) < 0)
    return -1;

  trace_enabled = enable;
  return 0;
}

int
sys_getprocs(void)
{
  return getprocs();
}

int
sys_proclist(void)
{
  proclist();
  return 0;
}

int
sys_syscount(void)
{
  int syscall_num;
  int *count_ptr;

  if(argint(0, &syscall_num) < 0)
    return -1;

  // Si syscall_num == -1, copiar todos los contadores al segundo argumento
  if(syscall_num == -1) {
    if(argptr(1, (void*)&count_ptr, sizeof(int)*26) < 0)
      return -1;
    
    int i;
    for(i = 0; i < 26; i++) {
      count_ptr[i] = syscall_counts[i];
    }
    return 0;
  }
  
  // Si no, retornar el contador del syscall específico
  if(syscall_num >= 0 && syscall_num < 26) {
    return syscall_counts[syscall_num];
  }
  
  return -1;
}
