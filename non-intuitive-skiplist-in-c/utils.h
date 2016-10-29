#ifndef __UTILS_H_
#define __UTILS_H_

typedef struct COMMANDS COMMANDS;

typedef enum{
	FALSE,
	TRUE
} boolean;

enum{
	INVALID = -1,
	EMPTY,
	UNITY
};

enum{
	ENTER = 10,
	CR_RETURN = 13,
	SPACEBAR = 32
};

enum{
	HOST,
	IP
};

enum{
	CMD_SEQSEARCH = 83,
	CMD_COMMANDS = 99,
	EXIT_PROGRAM = 101,
	CMD_INSERT = 105,
	CMD_MAPNODES = 109,
	CMD_REMOVE = 114,
	CMD_SEARCH = 115
};

COMMANDS *function_createCommands();
boolean function_getCommands(COMMANDS *);
boolean function_getValue(COMMANDS *);
boolean function_cleanCommands(COMMANDS *);
boolean function_destroyArgs(COMMANDS *);
boolean function_destroyCommands(COMMANDS **);
void function_dumpCommands();
char *function_getArgs(COMMANDS *, int);

#endif