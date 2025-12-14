#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int ticks, procs;

  ticks = uptime();
  procs = getprocs();

  printf(1, "uptime: %d ticks (%d segundos)\n", ticks, ticks/100);
  printf(1, "procesos activos: %d\n", procs);

  exit();
}
