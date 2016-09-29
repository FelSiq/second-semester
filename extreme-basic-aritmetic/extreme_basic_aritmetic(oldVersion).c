#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define BASE_DEC 10
#define MAX(A,B) A > B ? A : B
#define MIN(A,B) A < B ? A : B

enum{
    VEC_END = -20,
    INVALID = -1,
    EMPTY = 0,
    UNITY = 1,
    COMMANDS_COUNT = 4,
    ENTER = 10,
    ASCII0 = 48,
    AUX_SIZE = 50
};

enum{
    CASE_ADD,
    CASE_SUB,
    CASE_DIV,
    CASE_MUL
};

typedef short int TYPE;
typedef unsigned long long int LONG_TYPE;

typedef struct {
    TYPE *num1, *num2, *result;
    int num1_tam, num2_tam, res_tam;
    char num1_sig, num2_sig, res_sig;
} VEC_STRUCT;

typedef struct{
    char **string;
    int counter;
} COMMANDS;

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

COMMANDS *commands_create_structure(){
    COMMANDS *commands = malloc(sizeof(COMMANDS));
    if (commands != NULL){
        commands->string = NULL;
        commands->counter = EMPTY;
    }
    return commands;
}

void commands_destroy(COMMANDS **commands){
    int i;
    if (commands != NULL && *commands != NULL){
        for(i = EMPTY; i < (*commands)->counter; i++)
            free((*commands)->string[i]);
        free((*commands)->string);
        free(*commands);
        (*commands) = NULL;
    }
}

void commands_load(COMMANDS *commands){
    if(commands != NULL){
        commands->string = (char **) malloc(sizeof(char *) * COMMANDS_COUNT);

        commands->string[0] = (char *) malloc(6 * sizeof(char));
        strcpy(commands->string[0], "SOMA");

        commands->string[1] = (char *) malloc(11 * sizeof(char));
        strcpy(commands->string[1], "SUBTRACAO");

        commands->string[2] = (char *) malloc(9 * sizeof(char));
        strcpy(commands->string[2], "DIVISAO");

        commands->string[3] = (char *) malloc(15 * sizeof(char));
        strcpy(commands->string[3], "MULTIPLICACAO");

        commands->counter = COMMANDS_COUNT;
    }
}

int command_get(COMMANDS *commands, char *string, char *sig_a, char *sig_b){
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
    for(int i = EMPTY; i < commands->counter; i++)
        if(strcmp(commands->string[i], string) == EMPTY)
            return i;
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

void *vector_struct_destroy(VEC_STRUCT **vec_struct){
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
    for(int i = EMPTY; i < vec_struct->num1_tam; i++){
        if (vec_struct->num1[i] == vec_struct->num2[i])
            continue;
        if (vec_struct->num1[i] > vec_struct->num2[i])
            return vec_struct->num1;
        else
            return vec_struct->num2;
    }
    return vec_struct->num1;
}

TYPE *vector_check_smaller(VEC_STRUCT *vec_struct){
    if (vec_struct->num1_tam < vec_struct->num2_tam)
        return vec_struct->num1;
    if (vec_struct->num1_tam > vec_struct->num2_tam)
        return vec_struct->num2;
    for(int i = EMPTY; i < vec_struct->num1_tam; i++){
        if (vec_struct->num1[i] == vec_struct->num2[i])
            continue;
        if (vec_struct->num1[i] < vec_struct->num2[i])
            return vec_struct->num1;
        else
            return vec_struct->num2;
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
    else vec_struct->res_sig = EMPTY;

}

void vector_print(TYPE *vector, int signal, int size){
    if(signal == UNITY && !(size == UNITY && vector[EMPTY] == EMPTY)) printf("-");
    for(int i = (size - UNITY); i >= EMPTY; i--)
        printf("%d", vector[i]);
    printf("\n");
}

//A MULTIPLICACAO DE VETORES BASEA-SE NO ALGORITMO DE Karatsuba e Ofman.
//FONTE: http://www.ime.usp.br/~pf/analise_de_algoritmos/aulas/karatsuba.html
//Note que nenhum codigo foi extraido de lugar algum, o algoritmo estudado serviu apenas como base.

void vector_multiplication(VEC_STRUCT *vec_struct){
    int i, value_holder, max_size = MAX(vec_struct->num1_tam, vec_struct->num2_tam),
    min_size = MIN(vec_struct->num1_tam, vec_struct->num2_tam);
    int aux_unity1 = EMPTY, aux_unity2 = EMPTY;
    TYPE *aux_greater = vector_check_greater(vec_struct), *aux_smaller = vector_check_smaller(vec_struct);
    LONG_TYPE u_start = EMPTY, u_end = EMPTY, v_start = EMPTY, v_end = EMPTY, v_holder0, v_holder1;
    char *result_aux = NULL, *pr_aux = NULL, *qs_aux = NULL, *y_aux = NULL, *term_aux = NULL;

    if (max_size % 2 != EMPTY) aux_unity1 = UNITY;
    if (min_size % 2 != EMPTY) aux_unity2 = UNITY;

    if (max_size > UNITY){
        value_holder = (max_size/2);
        for(i = value_holder - UNITY + aux_unity1; i >= EMPTY; i--)
            u_end += aux_greater[i]*((LONG_TYPE) pow(BASE_DEC, i));
        for(i = max_size - UNITY; i >= value_holder + aux_unity1; i--)
            u_start += aux_greater[i]*((LONG_TYPE) pow(BASE_DEC, i - value_holder - aux_unity1/*- UNITY*/));
    } else u_start = aux_greater[EMPTY];

    if (min_size > UNITY){
        value_holder = (min_size/2);
        for(i = value_holder - UNITY + aux_unity2; i >= EMPTY; i--)
            v_end += aux_smaller[i]*((LONG_TYPE) pow(BASE_DEC, i));
        for(i = min_size - UNITY; i >= value_holder + aux_unity2; i--)
            v_start += aux_smaller[i]*((LONG_TYPE) pow(BASE_DEC, i - value_holder - aux_unity2/*- UNITY*/));
    } else v_end = aux_smaller[EMPTY];

    pr_aux = (char *) calloc(((min_size/2) + (max_size/2) + 2*UNITY), sizeof(char));
    sprintf(pr_aux, "%llu", (u_start*v_start));

    qs_aux = (char *) calloc(((min_size/2) + (max_size/2) + 2*UNITY), sizeof(char));
    sprintf(qs_aux, "%llu", u_end*v_end);

    y_aux = (char *) calloc((((min_size + 1)/2)*(max_size/2) + 3*UNITY), sizeof(char));
    sprintf(y_aux, "%llu", (u_start + u_end)*(v_start + v_end));

    term_aux = (char *) calloc((((min_size + 1)/2)*(max_size/2) + 3*UNITY), sizeof(char));
    sprintf(term_aux, "%llu", atoll(y_aux) - atoll(pr_aux) - atoll(qs_aux));

    result_aux = (char *) calloc((max_size + min_size + UNITY), sizeof(char));
    v_holder0 = (LONG_TYPE) pow(BASE_DEC, (max_size) + aux_unity1);
    v_holder1 = (LONG_TYPE) pow(BASE_DEC, (max_size)/2 + aux_unity1);
    sprintf(result_aux, "%llu", atoll(pr_aux)*v_holder0 + atoll(term_aux)*v_holder1 + atoll(qs_aux));

    vec_struct->res_sig = EMPTY;
    if (vec_struct->num1_sig != vec_struct->num2_sig)
        vec_struct->res_sig = UNITY;
    vec_struct->res_tam = strlen(result_aux);
    vector_char_to_int(&vec_struct->result, result_aux, &vec_struct->res_tam, EMPTY);

    free(y_aux);
    free(qs_aux);
    free(pr_aux);
    free(result_aux);
    free(term_aux);
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

int main(){
    VEC_STRUCT *vector_struct = vector_struct_create();
    COMMANDS *commands = commands_create_structure();
    char *OPERATOR = NULL;
    if(vector_struct != NULL){
        if(commands != NULL){
            commands_load(commands);

            OPERATOR = get_string();
            vector_struct->num1_sig = vector_get_number(&vector_struct->num1, &vector_struct->num1_tam);
            vector_struct->num2_sig = vector_get_number(&vector_struct->num2, &vector_struct->num2_tam);

            switch(command_get(commands, OPERATOR, &vector_struct->num1_sig, &vector_struct->num2_sig)){
                case CASE_ADD:
                    vector_add(vector_struct);
                    break;
                case CASE_SUB:
                    vector_subtration(vector_struct);
                    break;
                case CASE_MUL:
                    vector_multiplication(vector_struct);
                    break;
                case CASE_DIV:
                    vector_division(vector_struct);
                    break;
            }
            vector_print(vector_struct->result, vector_struct->res_sig, vector_struct->res_tam);
            commands_destroy(&commands);
        }
        vector_struct_destroy(&vector_struct);
    }
    return 0;
}
