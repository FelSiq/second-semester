#ifndef _UTILS_H_
#define _UTILS_H_

typedef enum{
	FALSE,
	TRUE
} boolean;

enum{
	INVALID = -1,
	EMPTY,
	UNITY,
	ENTER = 10,
	CARRIAGE_RETURN = 13,
	SPACE_BAR = 32,
	ASCIICOMMA = 44,
	AUX_MAX_SIZE = 250
};

char *get_string(int);
char **matrix_create(int, int);
char *get_keyword(char **, int);
boolean matrix_destroy(char **, short int);
boolean matrix_shift(char **, short int, short int);
boolean get_confirmation();

#endif