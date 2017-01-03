#ifndef __STACK_H_
#define __STACK_H_

#include "utils.h"

typedef struct stack STACK;
typedef struct node NODE;

STACK *stack_init();
void stack_push(STACK *, NODE *);
NODE *stack_pop(STACK *);
NODE *stack_top(STACK *);
void stack_destroy(STACK **);
bool stack_empty(STACK *);

#endif