#include <stdio.h>
#include <stdlib.h>
#include "googlebot.h"
#include <time.h>
#include <string.h>

char **matrix_create(int row, int col){
	char **matrix = (char **) malloc(sizeof(char *) * row);
	if (matrix != NULL)
		for(int i = EMPTY; i < row; i++)
			matrix[i] = (char *) malloc(sizeof(char) * (col + UNITY));
	return matrix;
}

boolean matrix_destroy(char **matrix, short int row){
	if (matrix != NULL){
		for(int i = EMPTY; i < row; i++)
			if (matrix[i] != NULL)
				free(matrix[i]);
		free(matrix);
		return TRUE;
	}
	return FALSE;
}

char *get_string(int max_lenght){
	char *string = NULL, c = EMPTY;
	short int counter = EMPTY;
	while(c != ENTER && c != CARRIAGE_RETURN && c != EOF){
		c = fgetc(stdin);
		string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
		string[counter++] = c;
	}
	if (max_lenght != INVALID && counter > max_lenght){
		string = (char *) realloc(string, sizeof(char) * (max_lenght + UNITY));
		string[max_lenght] = '\0';
		counter = max_lenght;
	}else string[counter - UNITY] = '\0';
	
	while(string[counter - 2] == SPACE_BAR || string[counter - 2] == '\n' || string[counter - 2] == CARRIAGE_RETURN){
		counter--;
		string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
		string[counter - UNITY] = '\0';
	}

	if(counter > UNITY)
		return string;
	else{
		free(string);
		return NULL;
	}
}

boolean matrix_shift(char **matrix, short int index, short int row){
	if(matrix != NULL){
		for(short int i = index; i < (row - UNITY); i++)
			matrix[i] = matrix[i + UNITY];
		return TRUE;
	}
	return FALSE;
}

boolean get_confirmation(){
	short int code_confirmation0 = INVALID, code_confirmation1 = EMPTY;
	srand(time(NULL));
	code_confirmation0 = (1000 + (rand() % 9000));
	printf("Are you SURE? Please type '%d' to confirm that action:\n", code_confirmation0);
	scanf("%hd", &code_confirmation1);
	getchar();

	if (code_confirmation0 == code_confirmation1)
		return TRUE;
	
	printf("Wrong code, cancelling previous action.\n");
	return FALSE;
}
/*AQUI!!!!*/
char *get_keyword(char **argv, int argc){
	char *string = NULL;
	short int counter = EMPTY, aux_size = EMPTY;
	for(int i = EMPTY; i < (argc - UNITY); i++){
		aux_size = strlen(argv[i + UNITY]); 
		counter += aux_size;
		string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
		memcpy(string, argv[i + UNITY], aux_size);
	}
	string[counter] = '\0';
	printf("TEST::: (%s)\n", string);
	return string;
}
