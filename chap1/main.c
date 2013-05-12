#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "slp.h"
#include "maxarg.h"
#include "prog1.h"

int main() {
  printf("%d\n", maxarg(prog()));
  
  return 0;
}
