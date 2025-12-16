#include "types.h"
#include "stat.h"
#include "user.h"

char *syscall_names[] = {
  [0]  "",
  [1]  "fork",
  [2]  "exit",
  [3]  "wait",
  [4]  "pipe",
  [5]  "read",
  [6]  "kill",
  [7]  "exec",
  [8]  "fstat",
  [9]  "chdir",
  [10] "dup",
  [11] "getpid",
  [12] "sbrk",
  [13] "sleep",
  [14] "uptime",
  [15] "open",
  [16] "write",
  [17] "mknod",
  [18] "unlink",
  [19] "link",
  [20] "mkdir",
  [21] "close",
  [22] "trace",
  [23] "getprocs",
  [24] "proclist",
  [25] "syscount",
};

int
main(int argc, char *argv[])
{
  if(argc == 2) {
    // Mostrar contador de un syscall específico
    int syscall_num = atoi(argv[1]);
    if(syscall_num < 0 || syscall_num > 25) {
      printf(2, "syscount: numero de syscall invalido (0-25)\n");
      exit();
    }
    
    int count = syscount(syscall_num);
    if(count < 0) {
      printf(2, "syscount: error al obtener contador\n");
      exit();
    }
    
    printf(1, "%s: %d invocaciones\n", syscall_names[syscall_num], count);
  }
  else if(argc == 1) {
    // Mostrar tabla resumen de todas las syscalls
    int counts[26];
    if(syscount(-1, counts) < 0) {
      printf(2, "syscount: error al obtener contadores\n");
      exit();
    }
    
    printf(1, "SYSCALL          INVOCACIONES\n");
    printf(1, "---------------  ------------\n");
    
    int i, j, len;
    for(i = 1; i < 26; i++) {
      if(counts[i] > 0) {
        printf(1, "%s", syscall_names[i]);
        len = strlen(syscall_names[i]);
        for(j = len; j < 15; j++) {
          printf(1, " ");
        }
        printf(1, "  %d\n", counts[i]);
      }
    }
  }
  else {
    printf(2, "uso: syscount [numero_syscall]\n");
  }
  
  exit();
}
