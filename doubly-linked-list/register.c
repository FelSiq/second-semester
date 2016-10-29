#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "register.h"

/*
 *	Esta função inicializa a estrutura de dados principal e cria o nó sentinela (1).
 *	Vale ressaltar que o setup da lista circular também ocorre (2).
 */
REG_STRUCT *reg_struct_create(){
    REG_STRUCT *reg_struct = (REG_STRUCT *) malloc(sizeof(REG_STRUCT));
    if(reg_struct != NULL){
        reg_struct->node_head = (REGISTER *) malloc(sizeof(REGISTER));/* (1) */
        if (reg_struct->node_head != NULL){
        	/* (2) */
            reg_struct->node_head->next = reg_struct->node_head;
            reg_struct->node_head->prev = reg_struct->node_head;
            reg_struct->node_head->nam_first = NULL;
            reg_struct->node_head->nam_last = NULL;
            reg_struct->node_head->cpf = NULL;
            reg_struct->node_head->tel = NULL;
        }
        return reg_struct;
    }
    return NULL;
}

/*
 *	As duas funções que seguem realizam a desalocação de mem. Heap.
 */

boolean register_destroy(REGISTER *reg){
    if (reg != NULL){
    	if (reg->nam_first != NULL)
    		free(reg->nam_first);
    	if (reg->nam_last != NULL)
    		free(reg->nam_last);
    	if (reg->cpf != NULL)
    		free(reg->cpf);
    	if (reg->tel != NULL)
    		free(reg->tel);

    	free(reg);
    	return TRUE;
    }
    return FALSE;
}

boolean reg_struct_destroy(REG_STRUCT **reg_struct){
    REGISTER *aux0, *aux1;
    if (reg_struct != NULL && *reg_struct != NULL){
        aux0 = (*reg_struct)->node_head->next;
        while(aux0 != (*reg_struct)->node_head){
            aux1 = aux0->next;
            register_destroy(aux0);
            aux0 = aux1;
        }
        register_destroy((*reg_struct)->node_head);
        free((*reg_struct));
        (*reg_struct) = NULL;
        return TRUE;
    }
    return FALSE;
}


/*
 *	Esta função obtém uma string, usada para obter os comandos. Note que, embora teoricamente não haja
 *	limites para uma string na heap, o truncamento automático de tamanho máximo 'solicitado' no PDF foi implementado (3).
 */

char *get_string(int max_len){
	char *string = NULL, c = EMPTY;
	int counter = EMPTY;
	while(c != ENTER && c != 13){
		c = fgetc(stdin);
		string = (char *) realloc(string, sizeof(char) * (counter + UNITY));
		string[counter++] = c;
	}
	//(3)
	if (max_len != INVALID && counter >= max_len){
		string = (char *) realloc(string, sizeof(char) * (max_len + UNITY));
		string[max_len] = '\0';
	}else string[counter - UNITY] = '\0';

	if (counter > UNITY)
		return string;
	else{
		free(string);
		return NULL;
	}
}

/*
 *	A função abaixo valida o comando digitado dentre os disponíveis.
 */

int get_command(char **aux){
	char *string;
	do string = get_string(INVALID);
	while (string == NULL);

	char *aux_adress = string, *aux_token;
	int aux_integer = INVALID, aux_size = EMPTY;
	aux_token = strtok(string, " ");
	if (strcmp(aux_token, "sair") == EMPTY){
		free(aux_adress);
		return CASE_EXIT;
	}

	if (strcmp(aux_token, "cadastrar") == EMPTY){
		free(aux_adress);
		return CASE_ADD;
	}

	if (strcmp(aux_token, "dump") == EMPTY){
		free(aux_adress);
		return CASE_DUMP;
	}
	if (strcmp(aux_token, "buscar") == EMPTY)
		aux_integer = CASE_SEARCH;
	if (strcmp(aux_token, "remover") == EMPTY)
		aux_integer = CASE_REMOVE;
	aux_token = strtok(NULL, "");

	if (aux_token != NULL){
		aux_size = strlen(aux_token);
		(*aux) = (char *) malloc(sizeof(char) * (aux_size + UNITY));
		memcpy((*aux), aux_token, aux_size);
		(*aux)[aux_size] = '\0';
	}

	if (aux_adress != NULL)
		free(aux_adress);
	return aux_integer;
}

/*
 * 	As duas próximas funções realizam a adição de um novo registro na lista.
 *	Note que a função imediatamente abaixo é apenas uma complementar da função principal, logo abaixo desta.
 */

REGISTER *register_autocreate(){
	REGISTER *reg = (REGISTER *) malloc(sizeof(REGISTER));
	if (reg != NULL){
		reg->nam_first = NULL;
		reg->nam_last = NULL;
		reg->cpf = NULL;
		reg->tel = NULL;
	}
	return reg;
}

/*
 * 	Função que obtém um registro de acordo com a chave (no caso, cpf). Utilizei a 
 *	técnica de impor a existência da chave solicitada, copiando-a ao nó sentinela (4).
 */

boolean reg_struct_add(REG_STRUCT *reg_struct){
	REGISTER *reg;
	char *aux;
	int max_len = MAX_DATA_RANGE;
	if(reg_struct != NULL){
		reg = register_autocreate();
		if (reg != NULL){
			do aux = get_string(max_len);
			while (aux == NULL);
			reg->nam_first = aux;

			do aux = get_string(max_len);
			while (aux == NULL);
			reg->nam_last = aux;

			do aux = get_string(max_len);
			while (aux == NULL);
			reg->cpf = aux;

			max_len = MAX_TEL_RANGE;
			do aux = get_string(max_len);
			while (aux == NULL);
			reg->tel = aux;

			reg->prev = reg_struct->node_head->prev;
			reg->next = reg_struct->node_head;
			reg_struct->node_head->prev->next = reg;
			reg_struct->node_head->prev = reg;

			return TRUE;
		}
	}
	return FALSE;
}

REGISTER *reg_struct_getregister(REG_STRUCT *reg_struct, char *key){
	REGISTER *aux = NULL;
	int aux_integer = EMPTY;
	if (reg_struct != NULL){
		aux_integer = strlen(key);
		reg_struct->node_head->cpf = (char *) malloc(sizeof(char) * (aux_integer + UNITY));
		memcpy(reg_struct->node_head->cpf, key, aux_integer);
		reg_struct->node_head->cpf[aux_integer] = '\0';

		aux = reg_struct->node_head->next;
		while(strcmp(key, aux->cpf) != EMPTY)
			aux = aux->next;
		
		free(reg_struct->node_head->cpf);
		reg_struct->node_head->cpf = NULL; 
		if (aux != reg_struct->node_head){
			return aux;
		}
	}
	return NULL;
}

/*
 * Remoção de um elemento da lista de acordo com o cpf solicitado.
 */

boolean reg_struct_remove(REG_STRUCT *reg_struct, char *key){
	REGISTER *aux = NULL;
	if (reg_struct != NULL){
		aux = reg_struct_getregister(reg_struct, key);
		if (aux != NULL){
			aux->prev->next = aux->next;
			aux->next->prev = aux->prev;
			if(register_destroy(aux))
				return TRUE;
		}
	}
	return FALSE;
}

/*
 * 	A função abaixo é apenas uma intemediária entre o usuário e a função que 
 * 	realmente realiza a busca de um registro, no caso, reg_struct_getregister().
 *	O principal motivo da existência da função abaixo é a necessidade de imprimir os dados do registro obtidos,
 *	o que não pode ser feito da reg_struct_getregister() pois ela também é utilizada na remoção de registros.
 */

boolean reg_struct_search(REG_STRUCT *reg_struct, char *key){
	REGISTER *aux = NULL;
	if (reg_struct != NULL){
		aux = reg_struct_getregister(reg_struct, key);
		if (aux != NULL){
			printf("Primeiro Nome: %s\nUltimo Nome: %s\nCPF: %s\nTelefone: %s\n-----------------------------\n", 
				aux->nam_first, aux->nam_last, aux->cpf, aux->tel);
			return TRUE;
		}
	}
	return FALSE;
}

/*
 * Por fim, dump.
 */

boolean reg_struct_dump(REG_STRUCT *reg_struct){
	REGISTER *aux = NULL;
	if (reg_struct != NULL){
		aux = reg_struct->node_head->next;
		if (aux != NULL){
			while (aux != reg_struct->node_head){
				printf("Primeiro Nome: %s\nUltimo Nome: %s\nCPF: %s\nTelefone: %s\n-----------------------------\n", 
					aux->nam_first, aux->nam_last, aux->cpf, aux->tel);
				aux = aux->next;
			}
			return TRUE;
		}
	}
	return FALSE;
}