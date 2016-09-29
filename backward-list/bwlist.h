#ifndef _BWLIST_H_
#define _BWLIST_H_

enum{
	OP_END = 102,
	OP_INSERT = 105,
	OP_REMOVE = 114
};

enum{
	INVALID = -1,
	EMPTY,
	UNITY
};

enum{
	SUCCESS,
	FAILURE
};

typedef enum {
	FALSE,
	TRUE
} boolean;

typedef struct node{
	int key, time;
	struct node *next, *back;
} NODE;

typedef struct{
	NODE *start, *end;
	int time;
} LIST_STRUCT;

LIST_STRUCT *listStruct_create();
boolean listStruct_destroy(LIST_STRUCT **);
boolean entries_clean(char *, int *, int *);
boolean entries_get(char *, int *, int *);
boolean listStruct_remove(LIST_STRUCT *, int);
boolean listStruct_add(LIST_STRUCT *, int, int);
boolean listStruct_print(LIST_STRUCT *);

#endif