#ifndef _UTILS_H_
#define _UTILS_H_

#include "item.h"

typedef enum{
	FALSE,
	TRUE
} boolean;

enum{
	INVALID = -1,
	EMPTY,
	UNITY
};

enum{
	ENTER = 10,
	CARRIAGE_RETURN = 13,
	SPACE_BAR = 32
};

ITEM *function_binSearch(ITEM **, int *, int, int);
char *get_string();

#endif