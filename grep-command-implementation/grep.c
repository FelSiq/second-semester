#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grep.h"

//Função para obter uma string, i.e. nomes de arquivos e linhas de texto. 
char *get_string(FILE *fp){
	char *string = NULL, c = EMPTY;
	int counter = EMPTY;
	while(c != EOF && c != ENTER && c != C_RETURN){
		c = fgetc(fp);
		string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
		string[counter++] = c;
	}

	if (counter > UNITY){
		//Aqui eu retiro barras de espaços no final da string.
		while(string[counter - 2] == SPACEBAR){
			string = (char *) realloc(string, sizeof(char) * (counter));
			string[counter - 2] = '\0';
			counter--;
		}
		string[counter - UNITY] = '\0';
		return string;
	}else{
		free(string);
		return NULL;
	}
}

//Esta estrutura auxiliar é usada para guardar os parâmetros do grep
STR_TOKENS *str_tokens_create(){
	STR_TOKENS *tokens = (STR_TOKENS *) malloc(sizeof(STR_TOKENS));
	if (tokens != NULL){
		tokens->string = NULL;
		tokens->counter = EMPTY;
	}
	return tokens;
}

//Esta função tokeniza uma string de comando e guarda na estrutura auxiliar do grep
void str_tokens_get(STR_TOKENS *tokens, char **string){
	char *str_adress, *str_token;
	int tok_size;
	if (tokens != NULL && string != NULL && *string != NULL){
		str_adress = *string;

		str_token = strtok(*string, " ");
		while(str_token != NULL){
			tok_size = strlen(str_token);
			tokens->string = (char **) realloc(tokens->string, sizeof(char *) * (tokens->counter + UNITY));
			tokens->string[tokens->counter] = (char *) malloc(sizeof(char) * (tok_size + UNITY));
			memcpy(tokens->string[tokens->counter], str_token, tok_size);
			tokens->string[tokens->counter][tok_size] = '\0';

			tok_size = EMPTY;
			(tokens->counter)++;
			str_token = strtok(NULL, " ");
		}

		free(str_adress);
		(*string) = NULL;
	}
}

#ifdef DEBUG
	//Função de DEBUG
	void str_tokens_print(STR_TOKENS *tokens){
		for(int i = EMPTY; i < tokens->counter; i++)
			printf("%d. %s\n", i + UNITY, tokens->string[i]);
		printf("\n");
	}

	//Função de DEBUG
	static void function_adress(char *adress, int size){
		for(int i = 0; i < size; i++)
			adress[i + UNITY] = adress[i];
		adress[size - UNITY] = NULL; 
	}

	//Função de DEBUG
	void function_printText(char **text, int size){
		for(int i = 0; i < size; i++)
			printf("%s\n", text[i]);
	}
#endif

//Função que lê o arquivo de busca
char **function_getText(FILE *fp, int *lines){
	char **text = NULL, *aux = NULL;
	while(!feof(fp)){
		aux = get_string(fp);
		if (aux != NULL){
			text = (char **) realloc(text, sizeof(char *) * (*lines + UNITY));
			text[*lines] = aux;
			(*lines)++;
		}
		aux = NULL;
	}
	return text;
}

//Função que libera a memória usada para o texto do arquivo
void function_destroyText(char ***text, int *lines){
	if (text != NULL && *text != NULL){
		for(int i = EMPTY; i < *lines; i++)
			if ((*text)[i] != NULL)
				free((*text)[i]);
		free(*text);
		(*text) = NULL;
		(*lines) = EMPTY;
	}
}

//Esta função verifica a presença de um token numa string maior ou de igual tamanho
boolean function_checkStrings(char *string, char *token){
	int str_size = strlen(string), tok_size = strlen(token), i, j = 0;
	if (str_size < tok_size)
		return FALSE;
	if (string != NULL && token != NULL){
		for(i = EMPTY; i <= (str_size - tok_size) && j != tok_size; i++)
			for(j = EMPTY; j < tok_size; j++)
				if (token[j] != string[i + j])
					break;
		if (j >= tok_size)
			return TRUE;
	}
	return FALSE;
}

//Esta função realiza o grep.
void str_tokens_process(STR_TOKENS *tokens, boolean *FLAG){
	int counter = EMPTY, n_lines = EMPTY, line_size = EMPTY;
	char **text = NULL, *line_aux = NULL, *token_aux = NULL, *adress_aux = NULL;
	FILE *fp = NULL;

	if (tokens != NULL && tokens->string != NULL){
		printf("%s\n", tokens->string[KEYWORD]);

		for(int i = 2; i < tokens->counter; i++){
			//Laço para cada arquivo solicitado numa palavra-chave
			fp = fopen(tokens->string[i], "r");
			if (fp != NULL){
				text = function_getText(fp, &n_lines);
				counter = EMPTY;

				for(int j = EMPTY; j < n_lines; j++){
					//Laço para cada linha do texto do arquivo solicitado
					counter++;
					line_size = strlen(text[j]);
					line_aux = (char *) malloc(sizeof(char) * (line_size + UNITY));
					memcpy(line_aux, text[j], line_size);
					line_aux[line_size] = '\0';
					adress_aux = line_aux;

					token_aux = strtok(line_aux, " .(),;[]{}<>*\"\'\t");
					while(token_aux != NULL){
						//Laço para cada palavra da linha atual
						if(function_checkStrings(token_aux, tokens->string[KEYWORD])){
							(*FLAG) = TRUE;
							printf("\n%s: %d", tokens->string[i], counter);
							//Laço para cada linha de impressão dentro da margem do parâmetro dado na entrada
							for(int z = j - atoi(tokens->string[1]); z <= j + atoi(tokens->string[1]); z++)
								if (z >= EMPTY)
									printf("\n%s", text[z]);
								printf("\n");
						}
						token_aux = strtok(NULL, " .(),;[]{}<>*\"\'\t");
					}
					free(adress_aux);
				}
				fclose(fp);
				fp = NULL;
			}
			function_destroyText(&text, &n_lines);
		}
	}
}

//Destrói a estrutura auxiliar para os comandos.
void str_tokens_destroy(STR_TOKENS **tokens){
	if(tokens != NULL && *tokens != NULL){
		if ((*tokens)->string != NULL){
			while((*tokens)->counter > EMPTY){
				((*tokens)->counter)--;
				if ((*tokens)->string[(*tokens)->counter] != NULL)
					free((*tokens)->string[(*tokens)->counter]);
			}
			free((*tokens)->string);
		}
		free(*tokens);
		(*tokens) = NULL;
	}
}