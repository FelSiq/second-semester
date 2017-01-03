#ifndef __ARITH_H_
#define __ARITH_H_

#define MAX_INT ((1 << sizeof(int)*8) - 1)
#define MAX(A,B) (A > B) ? A : B
#define MIN(A,B) (A < B) ? A : B

#include "utils.h"

typedef enum{
	NUMBER,
	OPERATOR
} data_t;

typedef struct item ITEM;
typedef char alg;

void make_operation(ITEM *, ITEM *, ITEM *);
void print_operation(ITEM *, ITEM *, ITEM *);
void item_destroy(ITEM **);
void print_value(ITEM *);
void get_entries();

bool func_isoperator(ITEM *);

#endif