#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

typedef char TYPE;

typedef struct{
	TYPE **matrix;
	char side_start, side_end;
	short int mat_row, mat_col, x_end, y_end;
} MAT_STRUCT;

typedef enum{
	FALSE,
	TRUE
} boolean;

enum{
	INVALID = -1,
	EMPTY,
	UNITY,
	PARAM_NUMBER = 4,
	ENTER = 10,
	ASCII0 = 48,
	AUX_SIZE = 50
};

MAT_STRUCT *matrix_struct_create(){
	MAT_STRUCT *mat_struct = (MAT_STRUCT *) malloc(sizeof(MAT_STRUCT));
	if (mat_struct != NULL){
		mat_struct->matrix = NULL;
		mat_struct->side_start = INVALID;
		mat_struct->side_end = INVALID;
		mat_struct->mat_row = EMPTY;
		mat_struct->mat_col = EMPTY;
	}
	return mat_struct;
}

TYPE **matrix_create(int row, int col){
	TYPE **matrix = (TYPE **) malloc(sizeof(TYPE *) * row);
	for(int i = EMPTY; i < row; i++)
		matrix[i] = (TYPE *) malloc(sizeof(TYPE) * (col + UNITY));
	return matrix;
}

void matrix_destroy(TYPE **matrix, int row){
	for(int i = EMPTY; i < row; i++)
		free(matrix[i]);
	free(matrix);
}

void matrix_struct_destroy(MAT_STRUCT **mat_struct){
	if (mat_struct != NULL && *mat_struct != NULL){
		matrix_destroy((*mat_struct)->matrix, (*mat_struct)->mat_row);
		free(*mat_struct);
		(*mat_struct) = NULL;
	}
}

char *get_string(){
	char *string = NULL, c = EMPTY;
	int counter = EMPTY;
	while(c != ENTER){
		c = fgetc(stdin);
		string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
		string[counter++] = c;
	}
	string[counter - UNITY] = '\0';
	return string;
}

void information_get(MAT_STRUCT *mat_struct){
	char *aux = get_string(), *token;

	token = strtok(aux, " ");
	for(int i = EMPTY; i < PARAM_NUMBER; i++){
		switch(i){
			case 0: mat_struct->side_start = *token; break;
			case 1: mat_struct->side_end = *token; break;
			case 2: mat_struct->mat_row = atoi(token); break;
			case 3: mat_struct->mat_col = atoi(token); break;
		}
		token = strtok(NULL, " ");
	}

	//printf("%c, %c, %d, %d\n", mat_struct->side_start, mat_struct->side_end, mat_struct->mat_row, mat_struct->mat_col);
	mat_struct->matrix = matrix_create(mat_struct->mat_row, mat_struct->mat_col);
	free(aux);
	for(int i = EMPTY; i < mat_struct->mat_row; i++){
		aux = get_string();
		for(int j = EMPTY; j < mat_struct->mat_col; j++)
			mat_struct->matrix[i][j] = aux[j*2];
		mat_struct->matrix[i][mat_struct->mat_col] = '\0';
		free(aux);
	}
}

boolean labyrinth_solve(MAT_STRUCT *mat_struct, short int y, short int x){
	//printf("Digging... x = %d\ty = %d\txend = %d\tyend = %d\n", x, y, mat_struct->x_end, mat_struct->y_end);
	if ((mat_struct->x_end == INVALID && mat_struct->y_end == y) || (mat_struct->y_end == INVALID && mat_struct->x_end == x))
		return TRUE;

	if (x > EMPTY && mat_struct->matrix[y][x - UNITY] == ASCII0){
		mat_struct->matrix[y][x] = '1';
		if (labyrinth_solve(mat_struct, y, x - UNITY))
			return TRUE;
		mat_struct->matrix[y][x] = '0';
	}
	if (x < mat_struct->mat_col - UNITY && mat_struct->matrix[y][x + UNITY] == ASCII0){
		mat_struct->matrix[y][x] = '1';
		if (labyrinth_solve(mat_struct, y, x + UNITY))
			return TRUE;
		mat_struct->matrix[y][x] = '0';
	}
	if (y > EMPTY  && mat_struct->matrix[y - UNITY][x] == ASCII0){
		mat_struct->matrix[y][x] = '1';
		if (labyrinth_solve(mat_struct, y - UNITY, x))
			return TRUE;
		mat_struct->matrix[y][x] = '0';
	}
	if (y < mat_struct->mat_row - UNITY && mat_struct->matrix[y + UNITY][x] == ASCII0){
		mat_struct->matrix[y][x] = '1';
		if (labyrinth_solve(mat_struct, y + UNITY, x))
			return TRUE;
		mat_struct->matrix[y][x] = '0';
	}
	return FALSE;
}

void get_initial_information(MAT_STRUCT *mat_struct, short int *side_limit, short int *x_start, short int *y_start){
	switch(mat_struct->side_start){
		case 'N':
			*x_start = EMPTY;
			*y_start = EMPTY;
			*side_limit = mat_struct->mat_col;
			break;
		case 'S':
			*x_start = EMPTY;
			*y_start = mat_struct->mat_row - UNITY;
			*side_limit = mat_struct->mat_col;
			break;
		case 'O':
			*x_start = EMPTY;
			*y_start = EMPTY;
			*side_limit = mat_struct->mat_row;
		case 'L':
			*x_start = mat_struct->mat_col - UNITY;
			*y_start = EMPTY;
			*side_limit = mat_struct->mat_row;
	}
	switch(mat_struct->side_end){
		case 'N':
			mat_struct->x_end = INVALID;
			mat_struct->y_end = EMPTY;
			break;
		case 'L':
			mat_struct->x_end = mat_struct->mat_col - UNITY;
			mat_struct->y_end = INVALID;
			break;
		case 'O':
			mat_struct->x_end = EMPTY;
			mat_struct->y_end = INVALID;
			break;
		case 'S':
			mat_struct->x_end = INVALID;
			mat_struct->y_end = mat_struct->mat_row - UNITY;
			break;
	}
}

int main(){
	MAT_STRUCT *mat_struct = matrix_struct_create();
	short int aux = EMPTY, side_limit = EMPTY, x_start = EMPTY, y_start = EMPTY;
	if (mat_struct != NULL){
		information_get(mat_struct);
		get_initial_information(mat_struct, &side_limit, &x_start, &y_start);

		while(aux < side_limit){
			//printf("XSTART = %d\nYSTART = %d\nMATRIX[X][Y] = %d\n", x_start, y_start, mat_struct->matrix[y_start][x_start]);
			if (mat_struct->matrix[y_start][x_start] == ASCII0){
				//printf("Found a possibility!\n");
				if(labyrinth_solve(mat_struct, y_start, x_start)){
					printf("Existe saida %c - %c.\n", mat_struct->side_start, mat_struct->side_end);
					break;
				}
				//printf("Didn't work...\n");
			}
			switch(mat_struct->side_start){
				case 'O':
				case 'L': y_start++; break;
				case 'N':
				case 'S': x_start++; break;
			}
			aux++;
			//printf("Will try a new start...\n");
		}
		if (aux >= side_limit)
			printf("Nao existe saida %c - %c.\n", mat_struct->side_start, mat_struct->side_end);
		matrix_struct_destroy(&mat_struct);
	}
	return 0;
}
