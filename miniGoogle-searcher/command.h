#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "utils.h"

typedef struct{
	char **command_string;
	short int command_value, command_counter;
} COMMAND_STRUCT;

enum{
	CASE_ABORT_SESSION,
	CASE_ADD_KEYWORD,
	CASE_ADD_SITE,
	CASE_COMMANDS,
	CASE_DUMP,
	CASE_EXIT,
	CASE_REFRESH_DATABASE,
	CASE_REFRESH_RELEVANCE,
	CASE_REMOVE_KEYWORD,
	CASE_REMOVE_SITE,
	CASE_SEARCH
};

COMMAND_STRUCT *command_getList();
short int command_value(COMMAND_STRUCT *);
short int command_count(COMMAND_STRUCT *);
boolean command_get(COMMAND_STRUCT *);
boolean command_destroyList(COMMAND_STRUCT **);
boolean command_list(COMMAND_STRUCT *);
boolean command_ForceInvalid(COMMAND_STRUCT *);

#endif