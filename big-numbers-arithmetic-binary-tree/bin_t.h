#ifndef __BIN_T_
#define __BIN_T_
#include "utils.h"
#include "arith.h"

typedef struct bin_t BIN_T;
typedef struct node NODE;

BIN_T *bint_init();
void bint_insert(BIN_T *, ITEM *);
void bint_print(BIN_T *);
void bint_destroy(BIN_T **);
void bint_solve(BIN_T *);

#endif