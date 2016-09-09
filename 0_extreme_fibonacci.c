#include <stdlib.h>
#include <stdio.h>

#define FIB_TERM_FIRST 0
#define FIB_TERM_SECOND 1
#define EMPTY 0
#define BASE 10

typedef short int TYPE;

typedef struct{
    TYPE *vec_a, *vec_b, *vec_r;
    int *size;
} STRUCT;

typedef enum{
    FALSE,
    TRUE
} boolean;

STRUCT *struct_create(){
    STRUCT *new_struct = malloc(sizeof(STRUCT));
    if (new_struct != NULL){
        new_struct->vec_a = NULL;
        new_struct->vec_b = NULL;
        new_struct->vec_r = NULL;
        new_struct->size = calloc(1, sizeof(int));

        return new_struct;
    }
    return NULL;
}

void struct_destroy(STRUCT **str){
    if (str != NULL && *str != NULL){
        if ((*str)->vec_a != NULL)
            free((*str)->vec_a);
        if ((*str)->vec_b != NULL)
            free((*str)->vec_b);
        if ((*str)->vec_r != NULL)
            free((*str)->vec_r);
        if ((*str)->size != NULL)
            free((*str)->size);
        free((*str));
        (*str) = NULL;
    }
}

void struct_setup_fibonacci(STRUCT *fib_struct){
    if (fib_struct != NULL){
        fib_struct->vec_a = (TYPE *) calloc(1, sizeof(TYPE));
        fib_struct->vec_b = (TYPE *) calloc(1, sizeof(TYPE));
        fib_struct->vec_r = (TYPE *) calloc(1, sizeof(TYPE));
        (*(fib_struct->size))++;
        *(fib_struct->vec_a) = FIB_TERM_FIRST;
        *(fib_struct->vec_b) = FIB_TERM_SECOND;
    }
}

void struct_vectors_sum(TYPE *vec_res, TYPE *vec_a, TYPE *vec_b, int size){
    for(int i = 0; i < size; i++)
        *(vec_res + i) = (*(vec_a + i) + *(vec_b + i));
}

void struct_vectors_update(STRUCT *str){
    (*(str->size))++;
    str->vec_r = (TYPE *) realloc(str->vec_r, sizeof(TYPE) * *(str->size));
        str->vec_r[*(str->size) - 1] = EMPTY;
    str->vec_a = (TYPE *) realloc(str->vec_a, sizeof(TYPE) * *(str->size));
        str->vec_a[*(str->size) - 1] = EMPTY;
    str->vec_b = (TYPE *) realloc(str->vec_b, sizeof(TYPE) * *(str->size));
        str->vec_b[*(str->size) - 1] = EMPTY;
}

void struct_vectors_check(STRUCT *str){
    for(int i = 0; i < *(str->size) - 1; i++){
        if(*(str->vec_r + i) >= BASE){
            *(str->vec_r + i + 1) += (*(str->vec_r + i))/BASE;
            *(str->vec_r + i) %= BASE;
        }
    }

    if (*(str->vec_r + *(str->size) - 1) >= BASE){
        struct_vectors_update(str);
        str->vec_r[*(str->size) - 1] = str->vec_r[*(str->size) - 2]/BASE;
        str->vec_r[*(str->size) - 2] %= BASE;
    }

}

void struct_vectors_copy(TYPE *vec_dest, TYPE *vec_from, int size){
    for(int i = 0; i < size; i++)
        *(vec_dest + i) = *(vec_from + i);
}

void struct_vectors_print_reverse(TYPE *vector, int size){
    if(vector != NULL)
        for(int i = (size - 1); i >= 0; i--)
            printf("%d", *(vector + i));
}

boolean general_check_exceptions(int n){
    if (n == 0) {
        printf("0");
        return TRUE;
    }
    if (n == 1) {
        printf("1");
        return TRUE;
    }
    return FALSE;
}

int main(){
    STRUCT *vectors;
    int n;

    scanf("%d", &n);
    getchar();

    if(!general_check_exceptions(n)){
        vectors = struct_create();
        struct_setup_fibonacci(vectors);

        while(n > 1){
            struct_vectors_sum(vectors->vec_r, vectors->vec_a, vectors->vec_b, *(vectors->size));
            struct_vectors_check(vectors);
            struct_vectors_copy(vectors->vec_a, vectors->vec_b, *(vectors->size));
            struct_vectors_copy(vectors->vec_b, vectors->vec_r, *(vectors->size));
            n--;
        }

        struct_vectors_print_reverse(vectors->vec_r, *(vectors->size));
        struct_destroy(&vectors);
    }

    return 0;
}
