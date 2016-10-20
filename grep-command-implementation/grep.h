#ifndef __GREP_H_
#define __GREP_H_H

typedef struct{
	char **string;
	short int counter; 
} STR_TOKENS;

enum{
	INVALID = -1,
	EMPTY,
	UNITY
};

enum{
	KEYWORD,
	PARAMETER
};

typedef enum{
	FALSE,
	TRUE
} boolean;

#define ENTER 10
#define C_RETURN 13
#define SPACEBAR 32
#define MAX_TEXT_LINE 250

char *get_string(FILE *);
STR_TOKENS *str_tokens_create();
void str_tokens_get(STR_TOKENS *, char **);
void str_tokens_process(STR_TOKENS *, boolean *);
void str_tokens_destroy(STR_TOKENS **);
void str_tokens_print(STR_TOKENS *);

#endif