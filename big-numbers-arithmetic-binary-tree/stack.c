#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "arith.h"
#include "utils.h"

typedef struct item{
	NODE *node;
	struct item *ant;
} ITEM;

struct stack{
	ITEM *top;
};

STACK *stack_init(){
	STACK *stack = (STACK *) malloc(sizeof(STACK));
	if (stack != NULL)
		stack->top = NULL;
	return stack;
};

static ITEM *func_itemInit(NODE *node, ITEM *ant){
	ITEM *item = (ITEM *) malloc(sizeof(ITEM));
	if (item != NULL){
		item->node = node;
		item->ant = ant;
	}
	return item;
};

void stack_push(STACK *stack, NODE *node){
	if (stack != NULL && node != NULL){
		ITEM *new_item = func_itemInit(node, stack->top);
		stack->top = new_item; 
	}
};

NODE *stack_pop(STACK *stack){
	if (stack != NULL){
		ITEM *item = stack->top;
		NODE *node = item->node;
		stack->top = stack->top->ant;
		free(item);
		return node;
	}
	return NULL;
};

NODE *stack_top(STACK *stack){
	if (stack != NULL && stack->top != NULL)
		return stack->top->node;
	return NULL;
};

void stack_destroy(STACK **stack){
	if (stack != NULL && *stack != NULL){
		ITEM *top = (*stack)->top, *rem = NULL;
		while(top != NULL){
			rem = top;
			top = (*stack)->top->ant;
			free(rem);
			rem = NULL;
		}
		free(*stack);
		(*stack) = NULL;
	}
};

bool stack_empty(STACK *stack){
	return (stack == NULL) ? TRUE : ((stack->top == NULL) ? TRUE : FALSE);
};
