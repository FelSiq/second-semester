#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#define ARGC_NUM 4

typedef enum{
    FALSE,
    TRUE
} boolean;


enum{
	SUCCESS,
	FAILURE
};

enum{
	ASCIIA = 65,
	ASCIIZ = 90
};

void merge_sort(char **, int, int);
void string_destroy(char **, int);
char **get_words(char *, int, int *);
boolean dump_dictionary(char *, char **, int);

#endif