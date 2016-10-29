#include <stdlib.h>
#include <stdio.h>
#include "register.h"

int main(int argc, char *argv[]){
	REG_STRUCT *reg_struct = reg_struct_create();
	int command = INVALID;
	char *aux = NULL;
	if (reg_struct != NULL){
		while(command != CASE_EXIT){
			command = get_command(&aux);
			switch(command){
				case CASE_DUMP:
					reg_struct_dump(reg_struct);
					break;
				case CASE_REMOVE:
					reg_struct_remove(reg_struct, aux);
					break;
				case CASE_SEARCH:
					reg_struct_search(reg_struct, aux);
					break;
				case CASE_ADD:
					reg_struct_add(reg_struct);
					break;
			}
			if (aux != NULL){
				free(aux);
				aux = NULL;
			}
		}
		reg_struct_destroy(&reg_struct);
	}

	if (aux != NULL)
		free(aux);
	
	return 0;
}