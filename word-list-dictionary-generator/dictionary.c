#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include "dictionary.h"

#define MAX_LENGHT 400

/*
 * This function is used to convert all uppercase letters to lowercase.
 */
void lower_letters(char *string, int size){
	for(int i = 0; i < size; i++)
		if(string[i] >= ASCIIA && string[i] <= ASCIIZ)
			string[i] += 32;
}

char **get_words(char *filename, int min_lenght, int *counter){
	char **string = NULL, *aux = NULL, *aux_token = NULL, *aux_adress = NULL;
	int aux_size = 0;
	if (filename != NULL && counter != NULL){
		FILE *fp = fopen(filename, "r");
		if (fp != NULL){
			while(!feof(fp)){
				aux = (char *) malloc(sizeof(char) * (MAX_LENGHT + 1));
				aux_adress = aux;
				fgets(aux, MAX_LENGHT, fp);
				if (aux != NULL){
					aux_token = strtok(aux, " ,.-_/()[]{}=+-:;!?*\n\r");
					while(aux_token != NULL){
						aux_size = strlen(aux_token);
						if (aux_size >= min_lenght){
							lower_letters(aux_token, aux_size);
							int i;
							for(i = 0; i < (*counter); i++)
								if (strcmp(aux_token, string[i]) == 0)
									break;
							if (i == *counter){
								string = (char **) realloc(string, sizeof(char *) * (*counter + 1));
								string[*counter] = (char *) malloc(sizeof(char) * (aux_size + 1));
								memcpy(string[*counter], aux_token, aux_size);
								string[*counter][aux_size] = '\0';
								(*counter)++;
							}
						}
						aux_token = strtok(NULL, " ,.-_/()[]{}=+-:;!?*\n\r");
					}
				if (aux_adress != NULL)
					free(aux_adress);
				}
			}
		}
	}
	return string;
}

void merge_ordenate(char **vector, int start, int middle, int end){
    printf("TEST!!!\n");
    char **aux = (char **) malloc(sizeof(char *) * (end - start + 1));
    int i = start, j = (middle + 1), k = 0;
    while(i <= middle || j < end){
        if (i > middle) aux[k++] = vector[j++];
        else{
            if(j > end) aux[k++] = vector[i++];
            else{
                if (strcmp(vector[i], vector[j]) < 0)
                    aux[k++] = vector[i++];
                else
                    aux[k++] = vector[j++];
            }
        } 
    }
    for(i = 0; i < k; i++)
        vector[start + i] = aux[i];
    free(aux);
}

void merge_sort(char **vector, int start, int end){
    if (end > start){
    	printf("CALL!!!\n");
        int middle = (start + end)/2;
        merge_sort(vector, start, middle);
        merge_sort(vector, middle + 1, end);
        merge_ordenate(vector, start, middle, end);
    }
}

void string_destroy(char **string, int rows){
	if (string != NULL && *string != NULL){
		for(int i = (rows - 1); i >= 0; i--)
			if (string[i] != NULL)
				free(string[i]);
		free(string);
	}
}

boolean dump_dictionary(char *filename, char **strings, int counter){
	if (filename != NULL && strings != NULL){
		FILE *fp = fopen(filename, "w");
		if (fp != NULL){
			int i;
			for(i = 0; i < (counter - 1); i++)
				fprintf(fp, "%s\n", strings[i]);
			fprintf(fp, "%s", strings[i]);
			return TRUE;
		}
	}
	return FALSE;
}