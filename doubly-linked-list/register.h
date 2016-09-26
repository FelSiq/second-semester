#ifndef _REGISTER_H_
#define _REGISTER_H_

#include "utils.h"

struct register_struct{
    char *nam_first, *nam_last, *cpf, *tel;
    struct register_struct *next, *prev;
};

typedef struct register_struct REGISTER;

typedef struct{
    REGISTER *node_head;
} REG_STRUCT;

REG_STRUCT *reg_struct_create();
boolean reg_struct_destroy(REG_STRUCT **);
boolean reg_struct_add(REG_STRUCT *);
boolean reg_struct_remove(REG_STRUCT *, char *);
boolean reg_struct_search(REG_STRUCT *, char *);
boolean reg_struct_dump(REG_STRUCT *);
char *get_string(int);
int get_command(char **);

#endif