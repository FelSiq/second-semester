#include <stdlib.h>
#include <stdio.h>
#include "commands.h"
#include <string.h>

COMMAND_STRUCT *command_structCreate(){
	COMMAND_STRUCT *command_struct = (COMMAND_STRUCT *) malloc(sizeof(COMMAND_STRUCT));
		if (command_struct != NULL){
			command_struct->param = NULL;
			command_struct->value = INVALID;
			command_struct->param_counter = EMPTY;
	}
	return command_struct;
}

boolean command_clearParameters(COMMAND_STRUCT *command_struct){
	if (command_struct != NULL){
		if (command_struct->param != NULL){
			free(command_struct->param);
			command_struct->param = NULL;
		}
		command_struct->value = INVALID;
		command_struct->param_counter = EMPTY;
		return TRUE;
	}
	return FALSE;
}

boolean command_structDestroy(COMMAND_STRUCT **command_struct){
	if (command_struct != NULL && *command_struct != NULL){
		command_clearParameters(*command_struct);
		free(*command_struct);
		(*command_struct) = NULL;
		return TRUE;
	}
	return FALSE;
}

int command_paramValue(COMMAND_STRUCT *command_struct){
	if(command_struct != NULL && command_struct->param != NULL)
		return command_struct->param[EMPTY];
	return INVALID;
}

boolean command_getCommand(COMMAND_STRUCT *command_struct){
	char *string = get_string(), *string_token = NULL, *string_adress = NULL;
	if (string != NULL){
		string_adress = string;
		string_token = strtok(string, " \n\r");
		command_clearParameters(command_struct);

		if (strcmp(string_token, "sair") == EMPTY){
			free(string_adress);
			command_struct->value = CASE_EXIT;
			return TRUE;
		}

		if (strcmp(string_token, "print") == EMPTY){
			free(string_adress);
			command_struct->value = CASE_PRINT;
			return TRUE;
		}

		if (strcmp(string_token, "tamanho") == EMPTY){
			free(string_adress);
			command_struct->value = CASE_SIZE;
			return TRUE;
		}

		if (strcmp(string_token, "vazia") == EMPTY){
			free(string_adress);
			command_struct->value = CASE_EMPTY;
			return TRUE;
		}

		if (strcmp(string_token, "shift") == EMPTY){
			command_struct->param = (int *) malloc(sizeof(int));
			string_token = strtok(NULL, " \n\r");
			if (string_token != NULL){
				command_struct->param[0] = atoi(string_token);
				free(string_adress);
				command_struct->value = CASE_SHIFT;
				return TRUE;
			}
		}

		if (strcmp(string_token, "contem") == EMPTY){
			command_struct->param = (int *) malloc(sizeof(int));
			string_token = strtok(NULL, " \n\r");
			if (string_token != NULL){
				command_struct->param[0] = atoi(string_token);
				free(string_adress);
				command_struct->value = CASE_CONTEM;
				return TRUE;
			}
		}

		if (strcmp(string_token, "remover") == EMPTY){
			command_struct->param = (int *) malloc(sizeof(int));
			string_token = strtok(NULL, " \n\r");
			if (string_token != NULL){
				command_struct->param[0] = atoi(string_token);
				free(string_adress);
				command_struct->value = CASE_REMOVE;
				return TRUE;
				
			}
		}

		if (strcmp(string_token, "get") == EMPTY){
			command_struct->param = (int *) malloc(sizeof(int));
			string_token = strtok(NULL, " \n\r");
			if (string_token != NULL){
				command_struct->param[0] = atoi(string_token);
				free(string_adress);
				command_struct->value = CASE_GET;
				return TRUE;
				
			}
		}

		if (strcmp(string_token, "indice") == EMPTY){
			command_struct->param = (int *) malloc(sizeof(int));
			string_token = strtok(NULL, " \n\r");
			if (string_token != NULL){
				command_struct->param[0] = atoi(string_token);
				free(string_adress);
				command_struct->value = CASE_INDICE;
				return TRUE;
			}
		}

		if (strcmp(string_token, "add") == EMPTY){
			command_struct->param = (int *) malloc(sizeof(int) * 2);
			string_token = strtok(NULL, " \n\r");
			if (string_token != NULL){
				command_struct->param[0] = atoi(string_token);
				string_token = strtok(NULL, " \n\r");
				if (string_token != NULL){
					command_struct->param[1] = atoi(string_token);
					free(string_adress);
					command_struct->value = CASE_ADD;
					return TRUE;
				}
			}
		}

		if (strcmp(string_token, "sub") == EMPTY){
			command_struct->param = (int *) malloc(sizeof(int) * 2);
			string_token = strtok(NULL, " \n\r");
			if (string_token != NULL){
				command_struct->param[0] = atoi(string_token);
				string_token = strtok(NULL, " \n\r");
				if (string_token != NULL){
					command_struct->param[1] = atoi(string_token);
					free(string_adress);
					command_struct->value = CASE_SUB;
					return TRUE;
				}
			}
		}

		if (strcmp(string_token, "set") == EMPTY){
			command_struct->param = (int *) malloc(sizeof(int) * 3);
			string_token = strtok(NULL, " \n\r");
			if (string_token != NULL){
				command_struct->param[0] = atoi(string_token);
				string_token = strtok(NULL, " \n\r");
				if (string_token != NULL){
					command_struct->param[1] = atoi(string_token);
					string_token = strtok(NULL, " \n\r");
					if (string_token != NULL){
						command_struct->param[2] = atoi(string_token);
						free(string_adress);
						command_struct->value = CASE_SET;
						return TRUE;
					}
				}
			}
		}

		free(string_adress);
	}
	return FALSE;
}

int command_getValue(COMMAND_STRUCT *command_struct){
	if(command_struct != NULL)
		return command_struct->value;
	return INVALID;
}