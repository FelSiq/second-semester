#ifndef __SPARSE_H_
#define __SPARSE_H_

#include "utils.h"

typedef struct node{
	int value;
	unsigned int i_col, i_row;
	struct node *right, *down;
} NODE;

typedef struct{
	NODE *node_master;
	unsigned int row_num, col_num;
} MATRIX;

MATRIX *matrix_create(unsigned int , unsigned int);
MATRIX *matrix_opAdd(MATRIX *, MATRIX *);
MATRIX *matrix_opMult(MATRIX *, MATRIX *);
boolean matrix_print(MATRIX *);
boolean matrix_destroy(MATRIX **);
boolean matrix_fill(MATRIX *, boolean *);

boolean matrix_nodeCount(MATRIX *);

#endif