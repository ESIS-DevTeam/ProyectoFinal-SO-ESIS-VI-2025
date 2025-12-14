#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int enable;
  
  if(argc != 2){
    printf(2, "uso: trace <1|0>\n");
    exit();
  }
  
  enable = atoi(argv[1]);
  
  if(enable != 0 && enable != 1){
    printf(2, "argumento debe ser 0 o 1\n");
    exit();
  }
  
  trace(enable);
  
  if(enable == 1){
    printf(1, "tracing activado\n");
  } else {
    printf(1, "tracing desactivado\n");
  }
  
  exit();
}
