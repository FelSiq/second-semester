#ifndef __ITEM_H_
#define __ITEM_H_

#include "utils.h"

typedef struct ITEM ITEM;

ITEM *function_createItem(char *, char *);
boolean function_itemDestroy(ITEM **);
char *function_getIp(ITEM *);
char *function_getHost(ITEM *);

#endif