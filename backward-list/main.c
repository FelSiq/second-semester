#include <stdlib.h>
#include <stdio.h>
#include "bwlist.h"

int main(int argc, char **argv){
	LIST_STRUCT *list_struct = listStruct_create();
	int back_index, key;
	char operator;

	entries_clean(&operator, &key, &back_index); 

	if (list_struct != NULL){
		while(operator != OP_END){
			entries_clean(&operator, &key, &back_index); 
			entries_get(&operator, &key, &back_index);

			switch(operator){
				case OP_INSERT:
					listStruct_add(list_struct, key, back_index);
					break;
				case OP_REMOVE:
					listStruct_remove(list_struct, key);
					break;
			}
		}
		
		listStruct_print(list_struct);
		listStruct_destroy(&list_struct);
		return SUCCESS;
	}
	return FAILURE;
}