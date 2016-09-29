#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "vectors.h"

char *get_string(){
    char *string = NULL, c = EMPTY;
    int counter = EMPTY;
    while(c != ENTER){
        c = fgetc(stdin);
        string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
        string[counter++] = c;
    }
    string[counter - 2*UNITY] = '\0';

    for(; counter >= EMPTY; counter--)
    	if (string[counter - UNITY] == SPACE_BAR){
    		counter--;
    		string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
    		string[counter - UNITY] = '\0';
    	}
    return string;
    	
}

int command_get(char *string, char *sig_a, char *sig_b){
    //SPECIAL CASES
    if(strcmp(string, "SUBTRACAO") == EMPTY && *sig_a == UNITY && *sig_b == EMPTY){
        (*sig_b) = UNITY;
        return CASE_ADD;
    }
    if(strcmp(string, "SOMA") == EMPTY && *sig_a == UNITY && *sig_b == EMPTY){
        return CASE_SUB;
    }
    if(strcmp(string, "SOMA") == EMPTY && *sig_a == EMPTY && *sig_b == UNITY){
        (*sig_b) = EMPTY;
        return CASE_SUB;
    }
    if(strcmp(string, "SUBTRACAO") == EMPTY && *sig_a == EMPTY && *sig_b == UNITY){
        (*sig_b) = EMPTY;
        return CASE_ADD;
    }
    //NORMAL CASES
    if(strcmp(string, "SOMA") == EMPTY) 
        return CASE_ADD;
    else{
    	    if(strcmp(string, "SUBTRACAO") == EMPTY) 
                return CASE_SUB;
	    else{
	    	    if(strcmp(string, "MULTIPLICACAO") == EMPTY) 
                    return CASE_MUL;
		    else{
		    	    if(strcmp(string, "DIVISAO") == EMPTY) 
                        return CASE_DIV;
		    }
	    }
    }
    return INVALID;
}

VEC_STRUCT *vector_struct_create(){
    VEC_STRUCT *vec_structure = (VEC_STRUCT *) malloc(sizeof(VEC_STRUCT));
    if (vec_structure != NULL){
        vec_structure->num1 = NULL;
        vec_structure->num2 = NULL;
        vec_structure->result = NULL;

        vec_structure->res_tam = EMPTY;
        vec_structure->num1_tam = EMPTY;
        vec_structure->num2_tam = EMPTY;

        vec_structure->res_sig = EMPTY;
        vec_structure->num1_sig = EMPTY;
        vec_structure->num2_sig = EMPTY;
    }
    return vec_structure;
}

void vector_struct_destroy(VEC_STRUCT **vec_struct){
    if (vec_struct != NULL && *vec_struct != NULL){
        free((*vec_struct)->num1);
        free((*vec_struct)->num2);
        free((*vec_struct)->result);
        free(*vec_struct);
        (*vec_struct) = NULL;
    }
}

void vector_char_to_int(TYPE **num_vec, char *num, int *size, int SIGNAL){
    int i;
    (*num_vec) = (TYPE *) malloc(sizeof(TYPE) * (*size + UNITY));
    for(i = (EMPTY); i < (*size); i++)
        (*num_vec)[(*size) - UNITY - i] = (num[i + SIGNAL] - ASCII0);
    (*num_vec)[i] = VEC_END;
}

int string_treatment(char *string){
    if (string != NULL)
        if(string[EMPTY] == '-' || string[EMPTY] == '+') return UNITY;
    return EMPTY;
}

int vector_get_number(TYPE **num_vec, int *size){
    char *string = get_string();
    int aux = string_treatment(string);
    (*size) = (strlen(string) - aux);
    vector_char_to_int(num_vec, string, size, aux);
    free(string);
    return aux;
}

void vector_copy_element(TYPE *vec_dest, TYPE *vec_from, int start, int end){
    while(start < end){
        vec_dest[start] = vec_from[start];
        start++;
    }
}

void vector_normalizer_plus(TYPE **num_vec, int *size){
    for(int i = EMPTY; i < (*size - UNITY); i++)
        if((*num_vec)[i] >= BASE_DEC){
            (*num_vec)[i + UNITY] += (*num_vec)[i]/BASE_DEC;
            (*num_vec)[i] %= BASE_DEC;
        }
    if (*size >= UNITY && (*num_vec)[*size - UNITY] >= BASE_DEC){
        (*num_vec) = (TYPE *) realloc((*num_vec), sizeof(TYPE) *(*size + UNITY));
        (*num_vec)[*size] = (*num_vec)[*size - UNITY]/BASE_DEC;
        (*num_vec)[*size - UNITY] %= BASE_DEC;
        (*size)++;
    }
}

void vector_add(VEC_STRUCT *vec_struct){
    int i = EMPTY;
    vec_struct->res_tam = MAX(vec_struct->num1_tam, vec_struct->num2_tam);
    vec_struct->result = (TYPE *) calloc((vec_struct->res_tam + UNITY), sizeof(TYPE));
    while(vec_struct->num1[i] != VEC_END && vec_struct->num2[i] != VEC_END){
        vec_struct->result[i] = vec_struct->num1[i] + vec_struct->num2[i];
        i++;
    }
    if (vec_struct->num1_tam > vec_struct->num2_tam)
        vector_copy_element(vec_struct->result, vec_struct->num1, i, vec_struct->num1_tam);
    else
        vector_copy_element(vec_struct->result, vec_struct->num2, i, vec_struct->num2_tam);
    vector_normalizer_plus(&vec_struct->result, &vec_struct->res_tam);

    if (vec_struct->num1_sig == UNITY && vec_struct->num2_sig == UNITY)
        vec_struct->res_sig = UNITY;
}

TYPE *vector_check_greater(VEC_STRUCT *vec_struct){
    if (vec_struct->num1_tam > vec_struct->num2_tam)
        return vec_struct->num1;
    if (vec_struct->num1_tam < vec_struct->num2_tam)
        return vec_struct->num2;
    for(int i = (vec_struct->num1_tam - UNITY); i >= EMPTY; i++){
        if (vec_struct->num1[i] != vec_struct->num2[i]){
            if (vec_struct->num1[i] > vec_struct->num2[i])
                return vec_struct->num1;
            else
                return vec_struct->num2;
        }
    }
    return vec_struct->num1;
}

TYPE *vector_check_smaller(VEC_STRUCT *vec_struct){
    if (vec_struct->num1_tam < vec_struct->num2_tam)
        return vec_struct->num1;
    if (vec_struct->num1_tam > vec_struct->num2_tam)
        return vec_struct->num2;
    for(int i = (vec_struct->num1_tam - UNITY); i >= EMPTY; i++){
        if (vec_struct->num1[i] != vec_struct->num2[i]){
            if (vec_struct->num1[i] < vec_struct->num2[i])
                return vec_struct->num1;
            else
                return vec_struct->num2;
        }
    }
    return vec_struct->num1;
}

void vector_normalizer_minus(TYPE **vector, int *size){
    int i;
    for(i = EMPTY; i < (*size - UNITY); i++)
        if ((*vector)[i] < EMPTY){
            (*vector)[i + UNITY] -= UNITY;
            (*vector)[i] += BASE_DEC;
        }
    if ((*vector)[*size - UNITY] < EMPTY)
        (*vector)[*size - UNITY] *= -UNITY;
    while((*vector)[(*size) - UNITY] == EMPTY){
        if ((*size) != UNITY){
            (*vector) = (TYPE *) realloc((*vector), sizeof(TYPE)*(*size));
            (*size)--;
        } else break;
    }
}

void vector_subtration(VEC_STRUCT *vec_struct){
    TYPE *vector_aux1, *vector_aux2;
    int i = EMPTY;
    char signal_smaller = EMPTY, signal_greater = EMPTY;
    vec_struct->res_tam = MAX(vec_struct->num1_tam, vec_struct->num2_tam);
    vec_struct->result = (TYPE *) calloc((vec_struct->res_tam + UNITY), sizeof(TYPE));

    vector_aux1 = vector_check_greater(vec_struct);
    vector_aux2 = vec_struct->num2;
    signal_smaller = vec_struct->num2_sig;
    signal_greater = vec_struct->num1_sig;
    if (vector_aux1 == vec_struct->num2){
        vector_aux2 = vec_struct->num1;
        signal_smaller = vec_struct->num1_sig;
        signal_greater = vec_struct->num2_sig;
    }

    while(vector_aux1[i] != VEC_END && vector_aux2[i] != VEC_END){
        vec_struct->result[i] = vector_aux1[i] - vector_aux2[i];
        i++;
    }

    if (vec_struct->num1_tam > vec_struct->num2_tam)
        vector_copy_element(vec_struct->result, vec_struct->num1, i, vec_struct->num1_tam);
    else
        vector_copy_element(vec_struct->result, vec_struct->num2, i, vec_struct->num2_tam);

    vector_normalizer_minus(&vec_struct->result, &vec_struct->res_tam);

    if (signal_greater == UNITY) vec_struct->res_sig = UNITY;
    else {
        if (signal_greater == EMPTY && signal_smaller == EMPTY && vector_aux1 == vec_struct->num2)
            vec_struct->res_sig = UNITY;
        else 
            vec_struct->res_sig = EMPTY;
    }

}

void vector_print(TYPE *vector, char signal, int size){
    if(signal == UNITY && !(size == UNITY && vector[EMPTY] == EMPTY)) printf("-");
    for(int i = (size - UNITY); i >= EMPTY; i--)
	   printf("%d", vector[i]);
    printf("\n");
}

void vector_add_unstructured(TYPE **vec_res, TYPE *vec_add, int *res_size, int add_size){
	for(int i = EMPTY; i < add_size; i++)
		(*vec_res)[i] += vec_add[i];
	vector_normalizer_plus(vec_res, res_size);
}

void vector_sub_unstructured(TYPE **vec_res, TYPE *vec_add, int *res_size, int add_size){
	for(int i = EMPTY; i < add_size; i++)
		(*vec_res)[i] -= vec_add[i];
	vector_normalizer_minus(vec_res, res_size);
}

void vector_multiplication(VEC_STRUCT *vec_struct){
	vec_struct->res_tam = (vec_struct->num1_tam + vec_struct->num2_tam);
	vec_struct->result = (TYPE *) calloc(vec_struct->res_tam, sizeof(TYPE));
	TYPE *aux_greater = vector_check_greater(vec_struct), *aux_smaller = vector_check_smaller(vec_struct);
	int aux_size_smaller = MIN(vec_struct->num2_tam, vec_struct->num1_tam), aux_size_greater = MAX(vec_struct->num2_tam, vec_struct->num1_tam);
	for(int i = EMPTY; i < aux_size_smaller; i++)
		for(int j = EMPTY; j < aux_size_greater; j++)
		vec_struct->result[i + j] += aux_greater[j]*aux_smaller[i];
		
	vector_normalizer_plus(&vec_struct->result, &vec_struct->res_tam);
	vector_normalizer_minus(&vec_struct->result, &vec_struct->res_tam);

	if (vec_struct->num1_sig == vec_struct->num2_sig) vec_struct->res_sig = EMPTY;
	else vec_struct->res_sig = UNITY;
}

int vector_check_zero(TYPE *vector, int size){
    for(int i = EMPTY; i < size; i++)
        if (vector[i] != EMPTY)
            return EMPTY;
    return UNITY;
}

void vector_division(VEC_STRUCT *vec_struct){
    int aux, j;
    if (vec_struct->num1_tam >= vec_struct->num2_tam && !vector_check_zero(vec_struct->num2, vec_struct->num2_tam)){
        aux = EMPTY;
        while(vec_struct->num1_tam >= vec_struct->num2_tam + vec_struct->res_tam){
            for(j = EMPTY; j < (vec_struct->num2_tam - UNITY); j++)
                vec_struct->num1[vec_struct->num2_tam - UNITY - j] -= vec_struct->num2[vec_struct->num2_tam - UNITY - j];
            vector_normalizer_minus(&vec_struct->num1, &vec_struct->num1_tam);
            aux++;
        }
        while (aux > EMPTY){
            vec_struct->result = (TYPE *) realloc(vec_struct->result, sizeof(TYPE) * (vec_struct->res_tam + UNITY));
            vec_struct->result[vec_struct->res_tam] = aux%BASE_DEC;
            aux /= BASE_DEC;
            (vec_struct->res_tam)++;
        }

        vec_struct->result = (TYPE *) realloc(vec_struct->result, sizeof(TYPE) * (vec_struct->res_tam + UNITY));
        vec_struct->result[vec_struct->res_tam] = VEC_END;

        vec_struct->res_sig = EMPTY;
        if (vec_struct->num1_sig != vec_struct->num2_sig)
            vec_struct->res_sig = UNITY;
    }else{
        vec_struct->result = malloc(sizeof(TYPE));
        *(vec_struct->result) = EMPTY;
        vec_struct->res_sig = EMPTY;
        (vec_struct->res_tam)++;
    }
}
