#include <stdlib.h>
#include <stdio.h>
#include "arith.h"
#include "bin_t.h"
#include "utils.h"
#include <string.h>

typedef union {
	char operator;
	alg *module;
} und_t;

struct item{
	data_t type;
	und_t value;
	short int size;
};

static ITEM *init_item(data_t type, und_t value, short int size){
	ITEM *item = (ITEM *) malloc(sizeof(ITEM));
	if (item != NULL){
		item->type = type;
		item->value = value;
		item->size = size;
	}
	return item;
};

bool func_isoperator(ITEM *item){
	if (item != NULL)
		return (item->type == OPERATOR);
	return FALSE;
};

static char *getString(FILE *const fp){
	char *string = NULL;
	if (fp != NULL){
		char c = EMPTY;
		unsigned int counter = EMPTY, aux_c = EMPTY;
		while (c != ENTER && c != CR && c != EOF){
			c = fgetc(stdin);
			if (counter == aux_c){
				aux_c = 2*aux_c + UNITY;
				string = (char *) realloc(string, sizeof(char) * aux_c);
			}
			string[counter++] = c;
		}
		if (counter > UNITY){
			string[counter - UNITY] = '\0';
			string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
		} else {
			if (string != NULL)
				free(string);
			string = NULL;
		}
	}
	#ifdef DEBUG
		printf("%s\n", string);
	#endif
	return string;
};

void get_entries(BIN_T *bin_t){
	if (bin_t != NULL){
		char *string = getString(stdin), *adress = string;
		if (string != NULL){
			int size;
			char *token = strtok(string, " ");
			ITEM *item = NULL;
			und_t value;
			while(token != NULL){
				if (*token == '*' || *token == '-' || *token == '/' || *token == '+'){
					value.operator = *token;
					item = init_item(OPERATOR, value, EMPTY);
				} else {
					size = strlen(token);
					value.module = (alg *) malloc(sizeof(alg) * (size));
					for(int aux = size - UNITY; aux >= EMPTY; --aux)
						value.module[aux] = (alg) (token[size - aux - UNITY] - ASCII0);
					//take off unnecessary zeros
					while(size > UNITY && value.module[size - UNITY] == EMPTY)
						--size;
					value.module = (alg *) realloc(value.module, sizeof(alg) * size);
					
					item = init_item(NUMBER, value, size);
				}
				bint_insert(bin_t, item);
				token = strtok(NULL, " ");
			}
			free(adress);
		}
	}
};

static alg *operation_add(ITEM *a, ITEM *b, short int *size_r){
	if (a != NULL && b != NULL && size_r != NULL){
		alg *r = NULL;
		//'a' must be the greater vector
		if (a->size < b->size)
			SWAP_V(a, b);
		(*size_r) = a->size;
		r = (alg *) malloc(sizeof(alg) * (a->size + UNITY));
		if (r != NULL){
			short int aux;
			*r = EMPTY;
			for(aux = EMPTY; aux < b->size; ++aux){
				r[aux + UNITY] = (r[aux] + a->value.module[aux] + b->value.module[aux]) / BASE_DEC;
				r[aux] = (r[aux] + a->value.module[aux] + b->value.module[aux]) % BASE_DEC;
			}
			//Continue to the greater size
			for(; aux < a->size; ++aux){
				r[aux + UNITY] = (r[aux] + a->value.module[aux])/BASE_DEC;
				r[aux] = (r[aux] + a->value.module[aux]) % BASE_DEC;
			}
			//Check last position
			if (r[a->size - UNITY] >= BASE_DEC){
				r[a->size] = r[a->size - UNITY] / BASE_DEC; 
				r[a->size - UNITY] %= BASE_DEC;
			}
			if (r[a->size] > EMPTY)
				++(*size_r);
			return r;
		}
	}
	return NULL;
};

static alg *operation_min(ITEM *a, ITEM *b, short int *size_r){
	if (a != NULL && b != NULL && size_r != NULL){
		alg *r = NULL;
		bool sig = FALSE;
		//'a' must be the greater vector
		if (a->size < b->size){
			SWAP_V(a, b);
			sig = TRUE;
		}
		(*size_r) = a->size;
		r = (alg *) malloc(sizeof(alg) * (a->size + UNITY));
		if (r != NULL){
			short int aux;
			*r = EMPTY;
			for(aux = EMPTY; aux < b->size; ++aux){
				if (a->value.module[aux] < b->value.module[aux]){
					r[aux] += BASE_DEC;
					r[aux + UNITY] = INVALID;
				} else r[aux + UNITY] = EMPTY;
				r[aux] = (r[aux] + a->value.module[aux] - b->value.module[aux]) % BASE_DEC;
			}
			//Continue to the greater size
			for(r[aux] = (r[aux] + a->value.module[aux]), ++aux; aux < a->size; ++aux)
				r[aux] = (a->value.module[aux]);
			//Remove unnecessary zeros
			while((*size_r) > UNITY && r[(*size_r) - UNITY] == EMPTY)
				--(*size_r);
			r = (alg *) realloc(r, sizeof(alg) * (*size_r));
			if (sig)
				r[*size_r - UNITY] *= (-1);
			return r;
		}
	}
	return NULL;
};

static alg *operation_mult(ITEM *a, ITEM *b, short int *size_r){
	if (a != NULL && b != NULL && size_r != NULL){
		alg *r = NULL;
		//'a' must be the smallest vector
		if (a->size > b->size)
			SWAP_V(a, b);
		(*size_r) = a->size + b->size;
		r = (alg *) malloc(sizeof(alg) * (*size_r + UNITY));
		if (r != NULL){
			*r = EMPTY;
			for(short int i = EMPTY; i < a->size; ++i)
				for(short int j = EMPTY; j < b->size; ++j){
					r[i + j + UNITY] = (r[i + j] + a->value.module[i] * b->value.module[j]) / BASE_DEC;
					r[i + j] = (r[i + j] + a->value.module[i] * b->value.module[j]) % BASE_DEC;
				}

			while((*size_r) > UNITY && r[(*size_r) - UNITY] == EMPTY)
				--(*size_r);
			r = (alg *) realloc(r, sizeof(alg) * (*size_r));
			return r;
		}
	}
	return NULL;
};

static alg *operation_div(ITEM *a, ITEM *b, short int *size_r){
	if (a != NULL && b != NULL && size_r != NULL){
		alg *r = NULL;
		r = (alg *) malloc(sizeof(alg));
		(*size_r) = UNITY; 
		*r = EMPTY;
		//Check if b != 0
		if (b->size == UNITY && *(b->value.module) == EMPTY){
			printf(" [DIVISION BY ZERO!] ");
			return r;
		}
		//Check if a < b (integer division)
		if (a->size < b->size){
			return r;
		} else if (a->size == b->size){
			for(short int aux = a->size - UNITY; aux >= EMPTY; --aux)
				if (b->value.module[aux] > a->value.module[aux])
					return r;
				else if (b->value.module[aux] < a->value.module[aux])
					break;
		}
		if (r != NULL){
			//while(a->size )
			return r;
		}
	}
	return NULL;
};

void make_operation(ITEM *r, ITEM *a, ITEM *b){
	if (r != NULL && a != NULL && b != NULL){
		switch(r->value.operator){
			case '+': r->value.module = operation_add(a, b, &r->size); break;
			case '-': r->value.module = operation_min(a, b, &r->size); break;
			case '*': r->value.module = operation_mult(a, b, &r->size); break;
			default: r->value.module = operation_div(a, b, &r->size); break;
		}
		r->type = NUMBER;
	}
};

void print_operation(ITEM *r, ITEM *a, ITEM *b){
	if (a != NULL && a->type == NUMBER)
		print_value(a);
	
	if (r != NULL && r->type == OPERATOR)
		printf(" %c ", r->value.operator);
	
	if (b != NULL && b->type == NUMBER)
		print_value(b);
};

void print_value(ITEM *item){
	if (item != NULL){
		if (item->type)
			printf("%c", item->value.operator);
		else for(short int aux = item->size - UNITY; aux >= EMPTY; --aux)
			printf("%hd", item->value.module[aux]);
	}
};

void item_destroy(ITEM **item){
	if (item != NULL && *item != NULL){
		if ((*item)->value.module != NULL)
			free((*item)->value.module);
		free(*item);
		(*item) = NULL;
	}
};