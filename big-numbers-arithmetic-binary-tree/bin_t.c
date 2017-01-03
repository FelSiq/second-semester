#include <stdlib.h>
#include <stdio.h>
#include "bin_t.h"
#include "stack.h"
#include "arith.h"

struct node{
	struct node *son_l, *son_r;
	ITEM *item;
};

struct bin_t{
	NODE *root;
	STACK *memory;
};

BIN_T *bint_init(){
	BIN_T *bin_t = (BIN_T *) malloc(sizeof(BIN_T));
	if (bin_t != NULL){
		bin_t->root = NULL;
		bin_t->memory = stack_init();
		if (bin_t->memory == NULL){
			free(bin_t);
			return NULL;
		}
	}
	return bin_t;
};

static NODE *node_init(ITEM *item){
	NODE *node = (NODE *) malloc(sizeof(NODE));
	if (node != NULL){
		node->item = item;
		node->son_l = NULL;
		node->son_r = NULL;
	}
	return node;
};

static void func_insert(BIN_T *bin_t, NODE *new){
	NODE *node = stack_pop(bin_t->memory);
	if (node->son_l == NULL){
		node->son_l = new;
		stack_push(bin_t->memory, node);
	} else 
		node->son_r = new;

	if(func_isoperator(new->item))
		stack_push(bin_t->memory, new);
};

void bint_insert(BIN_T *bin_t, ITEM *item){
	if (bin_t != NULL){
		NODE *node = node_init(item);
		if (node != NULL && bin_t->root != NULL)
			func_insert(bin_t, node);
		else {
			bin_t->root = node;
			stack_push(bin_t->memory, node);
		}
	}
};

static void func_print(NODE *node){
	if (node != NULL && node->son_l != NULL && node->son_r != NULL){
		if (node->son_l != NULL)
			printf("(");
		func_print(node->son_l);
		if (func_isoperator(node->item)){
			ITEM *aux_l = NULL, *aux_r = NULL;
			if (!func_isoperator(node->son_l->item))
				aux_l = node->son_l->item;
			if (!func_isoperator(node->son_r->item))
				aux_r = node->son_r->item;
			print_operation(node->item, aux_l, aux_r);
		}
		func_print(node->son_r);
		if (node->son_r != NULL)
			printf(")");
	}
};

void bint_print(BIN_T *bin_t){
	if (bin_t != NULL){
		func_print(bin_t->root);
	}
};

void bint_destroy(BIN_T **bin_t){
	if (bin_t != NULL && *bin_t != NULL){
		if ((*bin_t)->root != NULL)
			free((*bin_t)->root);
		if ((*bin_t)->memory != NULL)
			stack_destroy(&(*bin_t)->memory);
		free(*bin_t);
		(*bin_t) = NULL;
	}
};

static void node_destroy(NODE **node){
	if (node != NULL && *node != NULL){
		if ((*node)->item != NULL)
			item_destroy(&(*node)->item);
		free(*node);
		(*node) = NULL;
	}
};

static void func_solve(NODE *node){
	if (node != NULL && node->son_l != NULL && node->son_r != NULL){
		func_solve(node->son_l);
		func_solve(node->son_r);
		make_operation(node->item, node->son_l->item, node->son_r->item);
		node_destroy(&node->son_l);
		node_destroy(&node->son_r);
	}
};

void bint_solve(BIN_T *bin_t){
	if (bin_t != NULL){
		func_solve(bin_t->root);
		printf(" = ");
		print_value(bin_t->root->item);
		printf("\n");
	}
};