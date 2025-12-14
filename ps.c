#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "PID  STATE      NAME\n");
  printf(1, "---  ---------  ----\n");
  proclist();
  exit();
}
