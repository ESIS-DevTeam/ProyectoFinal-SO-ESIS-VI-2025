#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"

int trace_enabled = 0;

// User code makes a system call with INT T_SYSCALL.
// System call number in %eax.
// Arguments on the stack, from the user call to the C
// library system call function. The saved user %esp points
// to a saved program counter, and then the first argument.

// Fetch the int at addr from the current process.
int
fetchint(uint addr, int *ip)
{
  struct proc *curproc = myproc();

  if(addr >= curproc->sz || addr+4 > curproc->sz)
    return -1;
  *ip = *(int*)(addr);
  return 0;
}

// Fetch the nul-terminated string at addr from the current process.
// Doesn't actually copy the string - just sets *pp to point at it.
// Returns length of string, not including nul.
int
fetchstr(uint addr, char **pp)
{
  char *s, *ep;
  struct proc *curproc = myproc();

  if(addr >= curproc->sz)
    return -1;
  *pp = (char*)addr;
  ep = (char*)curproc->sz;
  for(s = *pp; s < ep; s++){
    if(*s == 0)
      return s - *pp;
  }
  return -1;
}

// Fetch the nth 32-bit system call argument.
int
argint(int n, int *ip)
{
  return fetchint((myproc()->tf->esp) + 4 + 4*n, ip);
}

// Fetch the nth word-sized system call argument as a pointer
// to a block of memory of size bytes.  Check that the pointer
// lies within the process address space.
int
argptr(int n, char **pp, int size)
{
  int i;
  struct proc *curproc = myproc();
 
  if(argint(n, &i) < 0)
    return -1;
  if(size < 0 || (uint)i >= curproc->sz || (uint)i+size > curproc->sz)
    return -1;
  *pp = (char*)i;
  return 0;
}

// Fetch the nth word-sized system call argument as a string pointer.
// Check that the pointer is valid and the string is nul-terminated.
// (There is no shared writable memory, so the string can't change
// between this check and being used by the kernel.)
int
argstr(int n, char **pp)
{
  int addr;
  if(argint(n, &addr) < 0)
    return -1;
  return fetchstr(addr, pp);
}

extern int sys_chdir(void);
extern int sys_close(void);
extern int sys_dup(void);
extern int sys_exec(void);
extern int sys_exit(void);
extern int sys_fork(void);
extern int sys_fstat(void);
extern int sys_getpid(void);
extern int sys_kill(void);
extern int sys_link(void);
extern int sys_mkdir(void);
extern int sys_mknod(void);
extern int sys_open(void);
extern int sys_pipe(void);
extern int sys_read(void);
extern int sys_sbrk(void);
extern int sys_sleep(void);
extern int sys_unlink(void);
extern int sys_wait(void);
extern int sys_write(void);
extern int sys_uptime(void);
extern int sys_trace(void);

static int (*syscalls[])(void) = {
[SYS_fork]    sys_fork,
[SYS_exit]    sys_exit,
[SYS_wait]    sys_wait,
[SYS_pipe]    sys_pipe,
[SYS_read]    sys_read,
[SYS_kill]    sys_kill,
[SYS_exec]    sys_exec,
[SYS_fstat]   sys_fstat,
[SYS_chdir]   sys_chdir,
[SYS_dup]     sys_dup,
[SYS_getpid]  sys_getpid,
[SYS_sbrk]    sys_sbrk,
[SYS_sleep]   sys_sleep,
[SYS_uptime]  sys_uptime,
[SYS_open]    sys_open,
[SYS_write]   sys_write,
[SYS_mknod]   sys_mknod,
[SYS_unlink]  sys_unlink,
[SYS_link]    sys_link,
[SYS_mkdir]   sys_mkdir,
[SYS_close]   sys_close,
[SYS_trace]   sys_trace,
};

static char *syscall_names[] = {
[SYS_fork]    "fork",
[SYS_exit]    "exit",
[SYS_wait]    "wait",
[SYS_pipe]    "pipe",
[SYS_read]    "read",
[SYS_kill]    "kill",
[SYS_exec]    "exec",
[SYS_fstat]   "fstat",
[SYS_chdir]   "chdir",
[SYS_dup]     "dup",
[SYS_getpid]  "getpid",
[SYS_sbrk]    "sbrk",
[SYS_sleep]   "sleep",
[SYS_uptime]  "uptime",
[SYS_open]    "open",
[SYS_write]   "write",
[SYS_mknod]   "mknod",
[SYS_unlink]  "unlink",
[SYS_link]    "link",
[SYS_mkdir]   "mkdir",
[SYS_close]   "close",
[SYS_trace]   "trace",
};

void
syscall(void)
{
  int num;
  int ret;
  int should_trace;
  struct proc *curproc = myproc();

  num = curproc->tf->eax;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    should_trace = (trace_enabled && num != SYS_write);
    
    if(should_trace) {
      cprintf("[%d] %s -> %s(", curproc->pid, curproc->name, syscall_names[num]);
    
    if(num == SYS_fork || num == SYS_wait || num == SYS_getpid || 
       num == SYS_uptime) {
      cprintf(")");
    }
    else if(num == SYS_exit) {
      cprintf(")");
    }
    else if(num == SYS_kill || num == SYS_dup || num == SYS_close || 
            num == SYS_sleep || num == SYS_chdir || num == SYS_unlink) {
      int arg = 0;
      argint(0, &arg);
      cprintf("%d)", arg);
    }
    else if(num == SYS_pipe) {
      int arg = 0;
      argint(0, &arg);
      cprintf("0x%x)", arg);
    }
    else if(num == SYS_read || num == SYS_write || num == SYS_fstat) {
      int fd = 0, arg2 = 0, arg3 = 0;
      argint(0, &fd);
      argint(1, &arg2);
      argint(2, &arg3);
      cprintf("%d, 0x%x, %d)", fd, arg2, arg3);
    }
    else if(num == SYS_open) {
      char *path = 0;
      int mode = 0;
      argstr(0, &path);
      argint(1, &mode);
      cprintf("\"%s\", %d)", path ? path : "", mode);
    }
    else if(num == SYS_exec) {
      char *path = 0;
      argstr(0, &path);
      cprintf("\"%s\", ...)", path ? path : "");
    }
    else if(num == SYS_sbrk) {
      int n = 0;
      argint(0, &n);
      cprintf("%d)", n);
    }
    else if(num == SYS_mknod) {
      char *path = 0;
      int major = 0, minor = 0;
      argstr(0, &path);
      argint(1, &major);
      argint(2, &minor);
      cprintf("\"%s\", %d, %d)", path ? path : "", major, minor);
    }
    else if(num == SYS_link) {
      int arg1 = 0, arg2 = 0;
      argint(0, &arg1);
      argint(1, &arg2);
      cprintf("%d, %d)", arg1, arg2);
    }
    else if(num == SYS_mkdir) {
      char *path = 0;
      argstr(0, &path);
      cprintf("\"%s\")", path ? path : "");
    }
    else {
      cprintf("...)");
    }
    }
    
    ret = syscalls[num]();
    curproc->tf->eax = ret;
    
    if(should_trace) {
      cprintf(" = %d\n", ret);
    }
  } else {
    cprintf("%d %s: unknown sys call %d\n",
            curproc->pid, curproc->name, num);
    curproc->tf->eax = -1;
  }
}
