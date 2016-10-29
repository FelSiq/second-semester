#ifndef _VECTORS_H_
#define _VECTORS_H_

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
    SPACE_BAR = 32,
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

typedef struct {
    TYPE *num1, *num2, *result;
    int num1_tam, num2_tam, res_tam;
    char num1_sig, num2_sig, res_sig;
} VEC_STRUCT;

VEC_STRUCT *vector_struct_create();
char *get_string();
int vector_check_zero(TYPE *, int);
int command_get(char *, char *, char *);
TYPE *vector_check_smaller(VEC_STRUCT *);
TYPE *vector_check_greater(VEC_STRUCT *);
int vector_get_number(TYPE **, int *);
void vector_add(VEC_STRUCT *);
void vector_subtration(VEC_STRUCT *);
void vector_multiplication(VEC_STRUCT *);
void vector_division(VEC_STRUCT *);
void vector_print(TYPE *, char, int);
void vector_struct_destroy(VEC_STRUCT **);
void vector_normalizer_minus(TYPE **, int *);
void vector_normalizer_plus(TYPE **, int *);
void vector_copy_element(TYPE *, TYPE *, int, int);
void vector_char_to_int(TYPE **, char *, int *, int);

#endif
