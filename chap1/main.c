#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "slp.h"
#include "maxarg.h"
#include "prog1.h"
#include "interp.h"

int main() {
  //printf("%d\n", maxarg(prog()));
  interp(prog());
  return 0;
}
