#include <stdlib.h>
#include <stdio.h>
#include "arraylist.h"

int main(int argc, char **argv){
	ARRAY_STRUCT *array_struct = arraylist_createStructure(), *array_aux = NULL;
	COMMAND_STRUCT *command_struct = command_structCreate();
	ITEM *item = NULL;
	if (array_struct != NULL){
		if (command_struct != NULL){
			while(command_getValue(command_struct) != CASE_EXIT){
				command_getCommand(command_struct);
				switch(command_getValue(command_struct)){
					case CASE_SHIFT:
						if (command_paramValue(command_struct) == EMPTY)
							arraylist_shiftLeft(array_struct, EMPTY);
						else
							arraylist_shiftRight(array_struct, EMPTY);
						break;
					case CASE_EMPTY:
						printf("%d\n", arraylist_checkEmpty(array_struct));
						break;
					case CASE_SIZE:
						printf("%d\n", arraylist_checkSize(array_struct));
						break;
					case CASE_PRINT:
						arraylist_dumpData(array_struct);
						break;
					case CASE_ADD:
						arraylist_addElement(array_struct, command_struct);
						break;
					case CASE_SUB:
						array_aux = arraylist_getSubvector(array_struct, command_struct);
						if (array_aux != NULL){
							arraylist_dumpData(array_aux);
							arraylist_destroyStructure(&array_aux);
						}
						break;
					case CASE_SET:
						arraylist_setValues(array_struct, command_struct);
						break;
					case CASE_CONTEM:
						printf("%d\n", arraylist_checkItem(array_struct, command_struct));
						break;
					case CASE_INDICE:
						arraylist_getIndex(array_struct, command_struct);
						break;
					case CASE_REMOVE:
						arraylist_removeItem(array_struct, command_struct);
						break;
					case CASE_GET:
						item = function_getIteminIndex(array_struct, command_paramValue(command_struct));
						if (item != NULL)
							printf("%d/%d\n", item->chave, item->valor);
						break;
				}
			}
			command_structDestroy(&command_struct);
		}
		arraylist_destroyStructure(&array_struct);
		return SUCCESS;
	}
	return FAILURE;
}