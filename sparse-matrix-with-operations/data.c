#include <stdlib.h>
#include <stdio.h>
#include "data.h"

DATA *get_data(boolean *FLAG_NEWMAT){
	DATA *data = (DATA *) malloc(sizeof(DATA));
	int verify = EMPTY, junk;

	if (data != NULL){
		if (!(*FLAG_NEWMAT))
			junk = scanf("%d", &verify);
		if (*FLAG_NEWMAT || verify == INVALID)
			junk = scanf("%u%u", &data->max_row, &data->max_col);
		else {
			free(data);
			data = NULL;
		}
		getchar();
	}
	return data;
}

boolean free_data(DATA **data){
	if (data != NULL && *data != NULL){
		free(*data);
		(*data) = NULL;
		return TRUE;
	}
	return FALSE;
}