#include "util.h"
#include "slp.h"
#include "interp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*環境コンストラクタ*/
Table_ Table(string id, int value, Table_ tail) {
  Table_ t = malloc(sizeof(*t));
  t -> id = id;
  t -> value = value;
  t -> tail = tail;
  return t;
}

/*環境の更新。前にあったものをそのまま放置*/
Table_ update(Table_ table, string id, int value) {
  return Table(id, value, table);
}

/*環境から探し出す。一番先にみつけたもの。*/
int lookup(Table_ t, string key) {
  Table_ tmp = t;
  while (tmp != NULL) {
    if (!strcmp(tmp -> id, key)) {
      return tmp -> value;
    } else {
      tmp = tmp -> tail;
    }
  }
  
  fprintf(stderr, "unknown variable %s\n", key);
  exit(EXIT_FAILURE);
}

/*文のインタプリタ*/
Table_ interpStm(A_stm stm, Table_ t) {
  switch (stm -> kind) {
  case A_compoundStm: {
    Table_ t1 = interpStm(stm -> u.compound.stm1, t);
    Table_ t2 = interpStm(stm -> u.compound.stm2, t1);
    return t2;
  }
  case A_assignStm: {
    struct IntAndTable intAndTable;
    intAndTable = interpExp(stm -> u.assign.exp, t);
    return update(intAndTable.t, stm -> u.assign.id, intAndTable.i);
  }
  case A_printStm: {
    return printExpList(stm -> u.print.exps, t).t;
  }
  }
}

/*式のインタプリタ*/
struct IntAndTable interpExp(A_exp exp, Table_ t) {
  struct IntAndTable t1;
  switch (exp -> kind) {
  case A_idExp: {
    t1.i = lookup(t, exp -> u.id);
    t1.t = t;
    return t1;
  }
  case A_numExp: {
    t1.i = exp -> u.num;
    t1.t = t;
    return t1;
  }

  case A_opExp: {
    struct IntAndTable t2;
    t2 = interpExp(exp -> u.op.left, t);
    struct IntAndTable t3 = interpExp(exp -> u.op.right, t2.t);
    t1.t = t3.t;

    A_binop binop = exp -> u.op.oper;
    
    switch (binop) {
    case A_plus: {
      t1.i = t2.i + t3.i;
      return t1;
    }
    case A_minus: {
      t1.i = t2.i - t3.i;
      return t1;
    }
    case A_times: {
      t1.i = t2.i * t3.i;
      return t1;
    }
    case A_div: {
      t1.i = t2.i / t3.i;
      return t1;
    }
    default : {
      fprintf(stderr, "unknown binary operator\n");
      exit(EXIT_FAILURE);
    }
    }  
  case A_eseqExp: {
    return interpExp(exp -> u.eseq.exp, interpStm(exp -> u.eseq.stm, t));
  }
  default :{
    fprintf(stderr, "unknown binary operator\n");
    exit(EXIT_FAILURE);
  }
  }
  }
}

/*式のリストのインタプリタ*/
struct IntAndTable printExpList(A_expList exp_list, Table_ t) {
  switch (exp_list -> kind) {
  case A_pairExpList: {
    struct IntAndTable t1 = interpExp(exp_list -> u.pair.head, t);
    printf("%d ", t1.i);
    return printExpList(exp_list -> u.pair.tail, t1.t);
  }
  case A_lastExpList: {
    struct IntAndTable t1 = interpExp(exp_list -> u.last, t);
    printf("%d\n", t1.i);
    return t1;
  }
  default:
    fprintf(stderr, "Expression list error");
    exit(EXIT_FAILURE);
  }

}

void interp(A_stm stm) {
  interpStm(stm, NULL);
} 
