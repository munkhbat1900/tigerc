#include "util.h"
#include "slp.h"
#include "maxarg.h"

#include <stdio.h>
#include <stdlib.h>

int maxarg(A_stm stm) {
  switch (stm -> kind) {
  case A_compoundStm: {
    int num1 = maxarg(stm -> u.compound.stm1);
    int num2 = maxarg(stm -> u.compound.stm2);
    if (num1 > num2) {
      return num1;
    } else {
      return num2;
    }
  }
    
  case A_assignStm: {
    return maxexp(stm -> u.assign.exp);
  }
  case A_printStm: { 
    return maxexp_list(stm -> u.print.exps);
  }
  default : {
    fprintf(stderr, "Expression error");
    exit(EXIT_FAILURE);
  }
  }
}

int maxexp(A_exp exp) {
  switch (exp -> kind) {
  case A_idExp:
  case A_numExp: return 1;
  case A_opExp: {
    int num1 = maxexp(exp -> u.op.left);
    int num2 = maxexp(exp -> u.op.right);
    if (num1 > num2) {
      return num1;
    } else {
      return num2;
    }
  } 
  case A_eseqExp: {
    int num1 = maxarg(exp -> u.eseq.stm);
    int num2 = maxexp(exp -> u.eseq.exp);
    if (num1 > num2) {
      return num1;
    } else {
      return num2;
    }
  }
  default:
    fprintf(stderr, "Expression error");
    exit(EXIT_FAILURE);
  }
}

int maxexp_list(A_expList exp_list) {
  switch (exp_list -> kind) {
  case A_pairExpList: {
    int num1 = maxexp(exp_list -> u.pair.head);
    int num2 = 1 + maxexp_list(exp_list -> u.pair.tail);
    if (num1 > num2) {
      return num1;
    } else {
      return num2;
    }
  }
  case A_lastExpList: {
    return maxexp(exp_list -> u.last);
  }
  default:
    fprintf(stderr, "Expression list error");
    exit(EXIT_FAILURE);
  }
}
