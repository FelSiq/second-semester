#include <stdlib.h>
#include <stdio.h>
#include "grep.h"

int main(int argc, char **argv){
	STR_TOKENS *tokens;
	FILE *fp;
	char *filename = NULL, *string;
	boolean FLAG = FALSE;
	
	filename = get_string(stdin);
	if (filename != NULL){
		fp = fopen(filename, "r");
		if (fp != NULL){
			//FILE OPENED
			while(!feof(fp)){
				//START-WHILE
				//Laço para cada linha de comando
				tokens = str_tokens_create();
				if (tokens != NULL){
					string = get_string(fp);
					if (string != NULL){
						str_tokens_get(tokens, &string);
						if (FLAG == TRUE){
							printf("\n");
							FLAG = FALSE;
						}
						str_tokens_process(tokens, &FLAG);
						if (FLAG == FALSE)
							printf("\n");
						free(string);
						string = NULL;
					}
				}
				str_tokens_destroy(&tokens);
				//END-WHILE
			}
			fclose(fp);
			fp = NULL;
			//FILE CLOSED
		}
		free(filename);
		filename = NULL;
	}
	return 0;
}