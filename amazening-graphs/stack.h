#ifndef __STACK_H_
#define __STACK_H_

#include "utils.h"
#include "maze.h"

typedef struct item ITEM;
typedef struct stack {
	ITEM *top;
} STACK;

STACK *stack_create();
CHAMBER *stack_pop(STACK *);
CHAMBER *stack_top(STACK *);
boolean stack_empty(STACK *);
void stack_push(STACK *, CHAMBER *);
void stack_destroy(STACK *);

#endif