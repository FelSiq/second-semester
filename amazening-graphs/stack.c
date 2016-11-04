#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

struct item{
	CHAMBER *memory;
	struct item *prev;
};

STACK *stack_create(){
	STACK *stack = (STACK *) malloc(sizeof(STACK));
	if (stack != NULL)
		stack->top = NULL;
	return stack;
};

CHAMBER *stack_pop(STACK *stack){
	if (stack != NULL){
		ITEM *item = stack->top;

		if (!stack_empty(stack))
			stack->top = stack->top->prev;

		if (item != NULL){
			CHAMBER *aux = item->memory;
			free(item);
			return aux;
		}
	}
	return NULL;
};

boolean stack_empty(STACK *stack){
	if (stack != NULL)
		return (stack->top == NULL);
	return TRUE;
};

static ITEM *function_itemInit(ITEM *prev, CHAMBER *memory){
	ITEM *item = (ITEM *) malloc(sizeof(ITEM));
	if (item != NULL){
		item->memory = memory;
		item->prev = prev;
	}
	return item;
};

void stack_push(STACK *stack, CHAMBER *memory){
	if (stack != NULL && memory != NULL){
		ITEM *item = function_itemInit(stack->top, memory);
		if (item != NULL)
			stack->top = item;
	}
};

void stack_destroy(STACK *stack){
	if (stack != NULL){
		ITEM *aux = NULL;
		while(stack->top != NULL){
			aux = stack->top;
			stack->top = stack->top->prev;
			free(aux);
			aux = NULL;
		}
		free(stack);
	}
};

CHAMBER *stack_top(STACK *stack){
	if (stack != NULL && stack->top != NULL)
		return stack->top->memory;
	return NULL;
}