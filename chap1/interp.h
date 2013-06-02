typedef struct table *Table_;

struct table {
  string id;
  int value;
  Table_ tail;
};

Table_ Table(string, int, Table_);

Table_ interpStm(A_stm, Table_);
Table_ update(Table_, string, int);
int lookup(Table_ t, string key);


struct IntAndTable {
  int i;
  Table_ t;
};

struct IntAndTable interpExp(A_exp, Table_);

void interp(A_stm);

struct IntAndTable printExpList(A_expList, Table_);
