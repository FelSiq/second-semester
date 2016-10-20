#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

//#define DEBUG

struct COMMANDS{
	char op, **args;
	unsigned int argc;
};

COMMANDS *function_createCommands(){
	COMMANDS *commands = (COMMANDS *) malloc(sizeof(COMMANDS));
	if(commands != NULL){
		commands->args = NULL;
		commands->op = INVALID;
		commands->argc = EMPTY;
	}
	return commands;
}

char *function_getString(){
	char *string = NULL, c = EMPTY;
	int counter = EMPTY;
	while(c != SPACEBAR && c != ENTER && c != EOF && c != CR_RETURN){
		c = fgetc(stdin);
		string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
		string[counter++] = c;
	}
	if (counter > UNITY){
		string[counter - UNITY] = '\0';
		return string;
	} else {
		free(string);
		return NULL;
	}
}

boolean function_opIsValid(char op){
	return (op == CMD_INSERT || op == CMD_REMOVE || op == CMD_SEARCH || 
		op == CMD_MAPNODES || op == CMD_SEQSEARCH || op == EXIT_PROGRAM || op == CMD_COMMANDS);
}

boolean function_getCommands(COMMANDS *commands){
	if(commands != NULL){
		scanf("%c", &commands->op);
		getchar();
		if(function_opIsValid(commands->op)){
			if (commands->op != EXIT_PROGRAM && commands->op != CMD_MAPNODES && commands->op != CMD_COMMANDS){
				commands->args = (char **) malloc(sizeof(char *) * 2);
				commands->args[HOST] = function_getString();
				commands->argc++;
				if (commands->op == CMD_INSERT){
					commands->args[IP] = function_getString();
					commands->argc++;
				}
			}
			#ifdef DEBUG
				printf("D: operator: %c, arguments: ", commands->op);
				if (commands->op != EXIT_PROGRAM && commands->op != CMD_MAPNODES && commands-op != CMD_COMMANDS){
					printf("%s", commands->args[HOST]);
					if (commands->op == CMD_INSERT)
						printf(", %s", commands->args[IP]);
					printf("\n");
				} else printf("noone\n");
			#endif
			return TRUE;
		}
	}
	return FALSE;
}

boolean function_getValue(COMMANDS *commands){
	if(commands != NULL)
		return commands->op;
	return EXIT_PROGRAM;
}

boolean function_cleanCommands(COMMANDS *commands){
	if(commands != NULL){
		if (commands->args != NULL){
			free(commands->args);
			commands->args = NULL;
		}
		commands->argc = EMPTY;
		commands->op = INVALID;
		return TRUE;
	}
	return FALSE;
}

boolean function_destroyArgs(COMMANDS *commands){
	if (commands != NULL){
		if (commands->args != NULL){
			while(commands->argc > EMPTY){
				free(commands->args[commands->argc - UNITY]);
				(commands->argc)--;
			}
			free(commands->args);
			commands->args = NULL;
		}
		return TRUE;
	}
	return FALSE;
}

boolean function_destroyCommands(COMMANDS **commands){
	if(commands != NULL && *commands != NULL){
		function_destroyArgs(*commands);
		free(*commands);
		(*commands) = NULL;
		return TRUE;
	}
	return FALSE;
}

char *function_getArgs(COMMANDS *commands, int value){
	if (commands != NULL && commands->args != NULL)
		return commands->args[value];
	return NULL;
}

void function_dumpCommands(){
	printf("'%c' to insert\n'%c' to remove\n'%c' to mapnodes\n'%c' to skipsearch\n'%c' to sequential search\n'%c' to exit program\n",
		CMD_INSERT, CMD_REMOVE, CMD_MAPNODES, CMD_SEARCH, CMD_SEQSEARCH, EXIT_PROGRAM);
}