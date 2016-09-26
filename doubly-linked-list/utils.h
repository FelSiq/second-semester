#ifndef _UTILS_H_
#define _UTILS_H_

enum{
    INVALID = -1,
    EMPTY,
    UNITY,
    ENTER = 10
};

enum{
	CASE_EXIT,
	CASE_DUMP,
	CASE_REMOVE,
	CASE_SEARCH,
	CASE_ADD
};

#define MAX_DATA_RANGE 15
#define MAX_TEL_RANGE 20

typedef enum{
    FALSE,
    TRUE
} boolean;

#endif