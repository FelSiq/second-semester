#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

#include "utils.h"
#include "item.h"
#include "commands.h"

#define ARRAY_INITIAL_SIZE 10

typedef struct node{
	ITEM **item;
	short int node_size, item_count, node_start;
	struct node *next;
} NODE;

typedef struct{
	NODE *node_start, *node_end;
	short int node_count;
} ARRAY_STRUCT;

enum{
	SUCCESS,
	FAILURE
};

ARRAY_STRUCT *arraylist_createStructure();
boolean arraylist_destroyStructure(ARRAY_STRUCT **);
boolean arraylist_checkEmpty(ARRAY_STRUCT *);
boolean arraylist_dumpData(ARRAY_STRUCT *);
boolean arraylist_addElement(ARRAY_STRUCT *, COMMAND_STRUCT *);
int arraylist_checkSize(ARRAY_STRUCT *);
ITEM *function_getItem(ARRAY_STRUCT *, COMMAND_STRUCT *);
boolean arraylist_checkItem(ARRAY_STRUCT *, COMMAND_STRUCT *);
boolean arraylist_getIndex(ARRAY_STRUCT *, COMMAND_STRUCT *);
boolean arraylist_setValues(ARRAY_STRUCT *, COMMAND_STRUCT *);
ARRAY_STRUCT *arraylist_getSubvector(ARRAY_STRUCT *, COMMAND_STRUCT *);
boolean arraylist_removeItem(ARRAY_STRUCT *, COMMAND_STRUCT *);
boolean arraylist_shiftLeft(ARRAY_STRUCT *, int);
boolean arraylist_shiftRight(ARRAY_STRUCT *, int);
ITEM *function_getIteminIndex(ARRAY_STRUCT *, int);

#endif