#include <stdlib.h>
#include <stdio.h>
#include "command.h"
#include <string.h>

COMMAND_STRUCT *command_createCommandStruct(){
	COMMAND_STRUCT *command_struct = (COMMAND_STRUCT *) malloc(sizeof(COMMAND_STRUCT));
	if (command_struct != NULL){
		printf("STATUS: Succesfully created command structure...\n");
		command_struct->command_string = NULL;
		command_struct->command_value = INVALID;
		command_struct->command_counter = EMPTY;
	}
	return command_struct;
}

COMMAND_STRUCT *command_getList(){
	printf("STATUS: Preparing commands structure...\n");
	COMMAND_STRUCT *command_struct = command_createCommandStruct();
	char *string_aux = NULL;
	int aux_size = EMPTY;
	if (command_struct != NULL){
		printf("STATUS: Trying to access Gbot Commands Database...\n");
		FILE *fp = fopen("Commands.gbot", "r");
		if (fp != NULL){
			while(!feof(fp)){
				string_aux = (char *) malloc(sizeof(char) * (AUX_MAX_SIZE + UNITY));
				fgets(string_aux, AUX_MAX_SIZE, fp);
				if (string_aux != NULL){
					if (strcmp(string_aux, "END") != EMPTY){
						command_struct->command_string = (char **) realloc(command_struct->command_string, sizeof(char *) * (command_struct->command_counter + UNITY));
						aux_size = strlen(string_aux);
						command_struct->command_string[command_struct->command_counter] = (char *) malloc(sizeof(char) * (aux_size));
						memcpy(command_struct->command_string[command_struct->command_counter], string_aux, aux_size - UNITY);
						command_struct->command_string[command_struct->command_counter][aux_size - UNITY] = '\0';
						command_struct->command_counter++;
					}
					free(string_aux);
					string_aux = NULL;
					aux_size = EMPTY;
				}
			}
			printf("STATUS: Process finished...\n");
			fclose(fp);
		}
	}
	return command_struct;
}

short int command_value(COMMAND_STRUCT *command_struct){
	if (command_struct != NULL)
		return command_struct->command_value;
	return INVALID;
}

short int command_count(COMMAND_STRUCT *command_struct){
	if (command_struct != NULL)
		return command_struct->command_counter;
	return INVALID;
}

boolean command_get(COMMAND_STRUCT *command_struct){
	int start, end, middle;
	char *string = get_string(INVALID);
	if (string != NULL){
		if (command_struct != NULL){
			start = EMPTY;
			end = (command_struct->command_counter - UNITY);
			while(end >= start){
				middle = ((start + end) / 2);
				if (strcmp(string, command_struct->command_string[middle]) == EMPTY){
					free(string);
					command_struct->command_value = middle;
					return TRUE;
				}else{
					if (strcmp(string, command_struct->command_string[middle]) < EMPTY)
						end = (middle - UNITY);
					else
						start = (middle + UNITY);
				}
			}
		}
		free(string);
	}
	return FALSE;
}

boolean command_destroyList(COMMAND_STRUCT **command_struct){
	if (command_struct != NULL && *command_struct != NULL){
		matrix_destroy((*command_struct)->command_string, (*command_struct)->command_counter);
		free(*command_struct);
		(*command_struct) = NULL;
		return TRUE;
	}
	return FALSE;
}

boolean command_list(COMMAND_STRUCT *command_struct){
	if (command_struct != NULL){
		if (command_struct->command_counter > UNITY){
			printf("List of commands avaliable:\n");
			for(int i = EMPTY; i < command_struct->command_counter; i++)
				printf("%d. '%s'\n", i + UNITY, command_struct->command_string[i]);
		} else printf("There's no commands avaliable.");
		return TRUE;
	}
	return FALSE;
}

boolean command_ForceInvalid(COMMAND_STRUCT *command_struct){
	if (command_struct != NULL){
		command_struct->command_value = INVALID;
		return TRUE;
	}
	return FALSE;
}