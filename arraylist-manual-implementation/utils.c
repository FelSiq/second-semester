#include <stdlib.h>
#include <stdio.h>
#include "arraylist.h"

ITEM *function_binSearch(ITEM **vector, int *middle, int size, int key){
	int start, end;
	if (vector != NULL){
		start = EMPTY;
		end = size - UNITY;
		while (end >= start){
			(*middle) = (start + end)/2;
			if (vector[*middle] != NULL){
				if (vector[*middle]->chave == key)
					return vector[*middle];
				else{
					if (vector[*middle]->chave > key)
						end = (*middle - UNITY);
					else
						start = (*middle + UNITY);
				}
			}else{
				if ((*middle + start) < key)
					end = (*middle - UNITY);
				else
					start = (*middle + UNITY);
			}
		}
	}
	return NULL;
}

char *get_string(){
	char *string = NULL, c = EMPTY;
	int counter = EMPTY;
	while(c != ENTER && c != CARRIAGE_RETURN && c != EOF){
		c = fgetc(stdin);
		string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
		string[counter++] = c;
	}
	string[counter - UNITY] = '\0';
	return string;
}