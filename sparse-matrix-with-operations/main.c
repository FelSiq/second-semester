#include <stdlib.h>
#include <stdio.h>
#include "sparse.h"
#include "data.h"

//Não pretendi encapsular a estrutura 'data', apenas a coloquei em um .c diferente para manter a organização da main,
//daí os acessos diretos de suas informações.

int main(int argc, char **argv){
	DATA *data = NULL;
	MATRIX *matrix_a = NULL, *matrix_b = NULL, *matrix_r = NULL;
	char op = INVALID;
	boolean FLAG_NEWMAT = FALSE;
	int junk;

	junk = scanf("%c", &op);
	getchar();

	//matrix A information
	data = get_data(&FLAG_NEWMAT);
	if (data != NULL){
		matrix_a = matrix_create(data->max_row, data->max_col);
		matrix_fill(matrix_a, &FLAG_NEWMAT);
		free_data(&data);
	}

	//matrix B information
	data = get_data(&FLAG_NEWMAT);
	if (data != NULL){
		matrix_b = matrix_create(data->max_row, data->max_col);
		matrix_fill(matrix_b, &FLAG_NEWMAT);
		free_data(&data);
	}

	if (matrix_a != NULL && matrix_b != NULL){
		switch(op){
			case OP_ADD: 
				if (matrix_a->col_num == matrix_b->col_num && matrix_a->row_num == matrix_b->row_num)
					matrix_r = matrix_opAdd(matrix_a, matrix_b);
				break;
			case OP_MULT: 
				if (matrix_a->col_num == matrix_b->row_num)
					matrix_r = matrix_opMult(matrix_a, matrix_b);
				break;
		}
		if (matrix_r != NULL){
			matrix_print(matrix_r);
			matrix_destroy(&matrix_r);
		}
		matrix_destroy(&matrix_a);
		matrix_destroy(&matrix_b);
	}
	return 0;
}