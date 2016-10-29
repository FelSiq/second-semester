#ifndef __DATAMAT_H_
#define __DATAMAT_H_

#include "utils.h"

typedef struct DATA{
	char op;
	unsigned int max_row, max_col;
} DATA;

#define OP_ADD 65 //ascii table 'A'
#define OP_MULT 77 //ascii table 'M'

DATA *get_data(boolean *);
boolean free_data(DATA **);

#endif