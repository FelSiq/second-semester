#include <stdlib.h>
#include <stdio.h>

/*
 * Olá! Eu serei seu guia através do meu código.
 */

typedef double TYPE;
typedef short int BOOL;
#define MAX_RESULT_SIZE 10

enum{
	INVALID = -1,
	EMPTY,
	UNITY,
    SPACE_BAR = 32,
    ASCII0 = 48,
    ASCII9 = 57
};

enum{
	OPEN_PARENTHESES = 40,
	CLOSE_PARENTHESES,
    OP_MULT,
    OP_PLUS,
    OP_MINUS = 45,
    DOT,
    OP_DIV
};


/*
 * A função abaixo recebe as entradas. Note que eu forço a string recebida ficar sempre entre parênteses por questões
 * de beneficiar futuramente meu método de resolução para o problema em questão.
 */

char *get_string(int *counter){
	char *string = (char *) malloc(sizeof(char)), c = EMPTY;
	string[(*counter)++] = OPEN_PARENTHESES;
	
	while(!feof(stdin)){
		c = fgetc(stdin);
		if (c != EOF){
			string = (char *) realloc(string, sizeof(char) * (*counter + 2));
			string[(*counter)++] = c;
		}
	}
	string[(*counter)++] = CLOSE_PARENTHESES;
	string[*counter] = '\0';
	return string;
}

/*
 * Esta é uma função auxiliar usada para remover espaços em branco de strings.
 */

void string_shift(char **string, int *size){
	int k = EMPTY;
	if (string != NULL && *string != NULL){
		while(*size > EMPTY && (*string)[k] != '\0'){
			if ((*string)[k] == SPACE_BAR){
				for(int j = k; j < (*size - UNITY); j++)
					(*string)[j] = (*string)[j + UNITY];
				(*size)--;
			}
			k++;
		}
	(*string) = (char *) realloc(*string, sizeof(char) * (*size + UNITY));
	if (*string != NULL)
		(*string)[*size] = '\0';
	}
}

/*
 * Por sua vez, esta função consegue extrair um valor (seja ele float, inteiro etc) do meio de uma string e retorná-lo.
 */

TYPE get_value(char **string, int i, int *size){
	char *string_token = NULL;
	int counter = EMPTY;
	TYPE value = EMPTY;
	if (string != NULL && (*string) != NULL && *size > EMPTY){
		while ((*string)[i] == SPACE_BAR)
			i++;
		while(((*string)[i] >= ASCII0 && (*string)[i] <= ASCII9) || (*string)[i] == DOT){
			string_token = (char *) realloc(string_token, sizeof(char) * (counter + 2));
			string_token[counter++] = (*string)[i];
			(*string)[i++] = SPACE_BAR;
		}
		string_token[counter] = '\0';
		value = strtod(string_token, NULL);
		free(string_token);
	}
	return value;
}

/*
 * Depois de muitas tentativas, percebi que a estrutura vetorial simples é provavelmente a candidata ideal
 * para resolver este tipo de problema. Por isso, acabei criando algumas funções que me ajudariam para
 * mexer com tal tipo de estrutura. Abaixo, tem-se a vector_shift(), que basicamente deleta uma casa do vetor.
 */

void vector_shift(TYPE **vector, int index, int *vec_len){
	for(int i = index; i < (*vec_len - UNITY); i++)
		(*vector)[i] = (*vector)[i + UNITY];
	(*vec_len)--;
	(*vector) = (TYPE *) realloc(*vector, sizeof(TYPE) * (*vec_len));
}

void vector_aux_shift(BOOL **vector, int index, int vec_len){
	for(int i = index; i < (vec_len - UNITY); i++)
		(*vector)[i] = (*vector)[i + UNITY];
	(*vector) = (BOOL *) realloc(*vector, sizeof(BOOL) * (vec_len - UNITY));
}

/*
 * Foi solicitado uma função recursiva para resolver o problema. Bem, eu criei duas. A função abaixo é apenas uma auxiliar.
 */

 void quad_shift(TYPE **vector, BOOL **aux_vet, int i, int *vec_len){
 	vector_aux_shift(aux_vet, i, *vec_len);
	vector_aux_shift(aux_vet, i, *vec_len);
	vector_shift(vector, i, vec_len);
	vector_shift(vector, i, vec_len);
 }

/*
 *  Esta é a primeira função recursiva para o cálculo da expressão airitmética. Ela resolve um único parênteses,
 *  e substitui o resultado em seu lugar no vetor.
 */

void solve_parentheses(TYPE **vector, BOOL **aux_vet, int *vec_len, int start, int end){
	/* Prioridade: multiplicaçao e divisao*/
	for(int i = start; i < end; i++){
		if((*vector)[i] == OP_DIV && (*aux_vet)[i] == UNITY){
			(*vector)[i - UNITY] /= (*vector)[i + UNITY];
			quad_shift(vector, aux_vet, i, vec_len);
			end -= 2;
			i--;
		}
		if((*vector)[i] == OP_MULT && (*aux_vet)[i] == UNITY){
			(*vector)[i - UNITY] *= (*vector)[i + UNITY];
			quad_shift(vector, aux_vet, i, vec_len);
			end -= 2;
			i--;
		}
	}
	/* segundo plano: adição e subtração*/
	for(int i = start; i < end; i++){
		if((*vector)[i] == OP_MINUS && (*aux_vet)[i] == UNITY){
			(*vector)[i - UNITY] -= (*vector)[i + UNITY];
			quad_shift(vector, aux_vet, i, vec_len);
			end -= 2;
			i--;
		}
		if((*vector)[i] == OP_PLUS && (*aux_vet)[i] == UNITY){
			(*vector)[i - UNITY] += (*vector)[i + UNITY];
			quad_shift(vector, aux_vet, i, vec_len);
			end -= 2;
			i--;
		}
	}
	if (end - start > 2)
		solve_parentheses(vector, aux_vet, vec_len, start, end); // <<<< A chamada está aqui.
	else{
		vector_aux_shift(aux_vet, start, *vec_len);
		vector_shift(vector, start, vec_len);
		end -=1;
		vector_aux_shift(aux_vet, end, *vec_len);
		vector_shift(vector, end, vec_len);
		end -=1;
	}

}

/*
 * A segunda função recursiva, calculator(), indica onde estão os parênteses e chama a função acima
 * até que a expressão aritmética esteja solucionada por completo.
 */

void calculator(TYPE **vector, BOOL **aux_vet, int *vec_len){
	if (*vec_len == UNITY)
		return;
	int i = EMPTY, end, start;
	while(i < *vec_len){
		if ((int) (*vector)[i] == CLOSE_PARENTHESES && (*aux_vet)[i] == UNITY)
			break;
		i++;
	}
	end = i;

	while(i >= EMPTY){
		if ((int) (*vector)[i] == OPEN_PARENTHESES && (*aux_vet)[i] == UNITY)
			break;
		i--;
	}
	start = i;

	solve_parentheses(vector, aux_vet, vec_len, start, end);
	calculator(vector, aux_vet, vec_len);
}

/*
 * Mais uma função para auxiliar na estrutura vetorial. Desta vez, fiz uma função que converte os valores extraídos
 * da string de input para um valor numérico no vetor.
 */

TYPE *fill_vector(char *string, int strsize, int *vec_size, BOOL **aux_vet){
	TYPE *vector = NULL;
	while(strsize > EMPTY){
		vector = (TYPE *) realloc(vector, sizeof(TYPE) * (*vec_size + UNITY));
		(*aux_vet) = (short int *) realloc(*aux_vet, sizeof(TYPE) * (*vec_size + UNITY));
		if(string[EMPTY] >= ASCII0 && string[EMPTY] <= ASCII9){
			(*aux_vet)[*vec_size] = EMPTY;
			vector[(*vec_size)++] = get_value(&string, EMPTY, &strsize);
		}
		if (string[EMPTY] >= OPEN_PARENTHESES && string[EMPTY] <= OP_DIV){
			if(string[EMPTY] == OP_MINUS && ((*vec_size) == EMPTY || (vector[(*vec_size) - UNITY] >= OPEN_PARENTHESES 
					&& vector[(*vec_size) - UNITY] <= OP_DIV && vector[(*vec_size) - UNITY] != CLOSE_PARENTHESES))){
				vector = (TYPE *) realloc(vector, sizeof(TYPE) * (*vec_size + 2));
				(*aux_vet) = (short int *) realloc(*aux_vet, sizeof(TYPE) * (*vec_size + 2));
				(*aux_vet)[*vec_size] = EMPTY;
				vector[(*vec_size)++] = -(1.0);
				(*aux_vet)[*vec_size] = UNITY;
				vector[(*vec_size)++] = OP_MULT;
			}else{
				(*aux_vet)[*vec_size] = UNITY;
				vector[(*vec_size)++] = string[EMPTY];
			}
			string[EMPTY] = SPACE_BAR;
		}

		if (string != NULL)
			string_shift(&string, &strsize);
	}
	return vector;
}

/*
 * Notei que o resultado não tem um número de casas significativas fixo, então
 * construí uma função que retirasse os "zeros" extras.
 */

void print_result(TYPE number){
	char *result = (char *) malloc(sizeof(char) * (MAX_RESULT_SIZE + UNITY));
	int res_size = sprintf(result, "%lf", number);

	for(int i = (res_size - UNITY); i > EMPTY; i--){
		if (result[i] == ASCII0 || result[i] == DOT){
			res_size--;
			if (result[i] == DOT)
				break;
		}
		else break;
	}

	for(int i = EMPTY; i < res_size; i++)
		printf("%c", result[i]);
	printf("\n");

	free(result);
}

/*
 * E isto é uma main.
 */

int main(){
	int counter = EMPTY, vec_size = EMPTY;
	TYPE *vector = NULL;
	BOOL *aux_vet = NULL;
	char *string = NULL;
	string = get_string(&counter);
	string_shift(&string, &counter);

	vector = fill_vector(string, counter, &vec_size, &aux_vet);

	calculator(&vector, &aux_vet, &vec_size);
	print_result(vector[EMPTY]);

	free(aux_vet);
	free(vector);
	return 0;
}

/*
 * Fim, praise the sun.
 */