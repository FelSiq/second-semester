#include <stdlib.h>
#include <stdio.h>
#include "vectors.h"

int main(int argc, char *argv[]){
    VEC_STRUCT *vector_struct = vector_struct_create();
    char *OPERATOR = NULL;
    if(vector_struct != NULL){
        OPERATOR = get_string();
        vector_struct->num1_sig = vector_get_number(&vector_struct->num1, &vector_struct->num1_tam);
        vector_struct->num2_sig = vector_get_number(&vector_struct->num2, &vector_struct->num2_tam);
        switch(command_get(OPERATOR, &vector_struct->num1_sig, &vector_struct->num2_sig)){
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
    }
    return 0;
}
