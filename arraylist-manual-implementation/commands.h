#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "utils.h"

enum{
	CASE_EXIT,
	CASE_SIZE,
	CASE_EMPTY,
	CASE_PRINT,
	CASE_ADD,
	CASE_SUB,
	CASE_SET,
	CASE_CONTEM,
	CASE_INDICE,
	CASE_REMOVE,
	CASE_GET,
	CASE_SHIFT
};

typedef struct{
	int *param, value, param_counter;
} COMMAND_STRUCT;

COMMAND_STRUCT *command_structCreate();
int command_paramValue(COMMAND_STRUCT *);
boolean command_clearParameters(COMMAND_STRUCT *);
boolean command_structDestroy(COMMAND_STRUCT **);
boolean command_getCommand(COMMAND_STRUCT *);
int command_getValue(COMMAND_STRUCT *);

#endif