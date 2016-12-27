#include <stdlib.h>
#include <stdio.h>
#include "deque.h"

//This is just a regular deque, used in my dynamic heap.

//He works like a cache memory, keeping the possible next fathers in something like 
//a priority queue. The exacly next father is always at the start, making the insertion
//on dynamic heap a extremely precise operation. Of course, this is not free, as the deque
//necessarily doubles the cost of leaf nodes on the dynamic heap. 

//No crazy ideas below, so absence of comments.

typedef struct d_item{
	struct d_item *prev, *next;
	NODE *node;
} D_ITEM;

struct deque{
	D_ITEM *last, *start;
};

DEQUE *deque_init(){
	DEQUE *deque = (DEQUE *) malloc(sizeof(DEQUE));
	if(deque != NULL){
		deque->last = NULL;
		deque->start = NULL;
	}
	return deque;
};

bool deque_empty(DEQUE *deque){
	if (deque != NULL && deque->last != NULL)
		return FALSE;
	return TRUE;
};

NODE *deque_pop_start(DEQUE *deque){
	if (deque != NULL && deque->start != NULL){
		NODE *node = deque->start->node;
		D_ITEM *aux = deque->start;
		deque->start = deque->start->prev;
		if (deque->start == NULL)
			deque->last = NULL;
		else
			deque->start->next = NULL;
		free(aux);

		return node;
	}
	return NULL;
};

NODE *deque_pop_end(DEQUE *deque){
	if (deque != NULL && deque->last != NULL){
		NODE *node = deque->last->node;
		D_ITEM *aux = deque->last;
		deque->last = deque->last->next;
		if (deque->last == NULL)
			deque->start = NULL;
		else
			deque->last->prev = NULL;
		free(aux);

		return node;
	}
	return NULL;
};

static D_ITEM *item_init(NODE *node, D_ITEM *prev, D_ITEM *next){
	D_ITEM *item = (D_ITEM *) malloc(sizeof(D_ITEM));
	if (item != NULL){
		item->node = node;
		item->prev = prev;
		item->next = next;
	}
	return item;
};

void deque_push_end(DEQUE *deque, NODE *node){
	if (deque != NULL && node != NULL){
		D_ITEM *item = item_init(node, NULL, deque->last);
		if (deque->last == NULL && deque->start == NULL){
			deque->last = item;
			deque->start = item;
		} else {
			deque->last->prev = item;
			deque->last = item;
		}
	}
};

void deque_push_start(DEQUE *deque, NODE *node){
	if (deque != NULL && node != NULL){
		D_ITEM *item = item_init(node, deque->start, NULL);
		if (deque->last == NULL && deque->start == NULL){
			deque->last = item;
			deque->start = item;
		} else{
			deque->start->next = item;
			deque->start = item;
		}
	}
};

void deque_destroy(DEQUE *deque){
	if (deque != NULL){
		D_ITEM *traveller = deque->start, *aux = NULL;
		while(traveller != NULL){
			aux = traveller;
			traveller = traveller->prev;
			free(aux);
		}
		free(deque);
	}
};