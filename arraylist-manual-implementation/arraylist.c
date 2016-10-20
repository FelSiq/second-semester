#include <stdlib.h>
#include <stdio.h>
#include "arraylist.h"

/* Função de criar e iniciar estrutura de Arrays */
ARRAY_STRUCT *arraylist_createStructure(){
	ARRAY_STRUCT *array_struct = (ARRAY_STRUCT *) malloc(sizeof(ARRAY_STRUCT));
	if (array_struct != NULL){
		array_struct->node_start = NULL;
		array_struct->node_end = NULL;
		array_struct->node_count = EMPTY;
	}
	return array_struct;
}
/* Função de criar e iniciar nó */
NODE *function_nodeAutocreate(){
	NODE *node = (NODE *) malloc(sizeof(NODE));
	if (node != NULL){
		node->item = NULL;
		node->item_count = EMPTY;
		node->node_size = EMPTY;
		node->node_start = INVALID;
		node->next = NULL;
	}
	return node;
}

/* Esta funções obtém um node específico apartir de uma dada posição, e retorna seu endereço. */
NODE *function_getNode(ARRAY_STRUCT *array_struct, int pos){
	NODE *node = NULL;
	if (array_struct != NULL){
		node = array_struct->node_start;
		while(node != NULL && pos > (node->node_size + node->node_start))
			node = node->next;
	}
	return node;
}

/* Função de criar e iniciar item. */
ITEM *function_itemAutocreate(){
	ITEM *item = (ITEM *) malloc(sizeof(ITEM));
	if (item != NULL){
		item->chave = INVALID;
		item->valor = EMPTY;
	}
	return item;
}

/* Esta função serve para criar, especificamente, o primeiro nó da estrutura de dados. */
boolean function_firstnodeCreation(ARRAY_STRUCT *array_struct){
	NODE *first_node = function_nodeAutocreate();
	if (array_struct != NULL && first_node != NULL){
		(array_struct->node_count++);
		array_struct->node_start = first_node;
		array_struct->node_end = first_node;

		first_node->node_size = (ARRAY_INITIAL_SIZE);

		first_node->node_start = EMPTY;
		first_node->item_count = EMPTY;
		first_node->next = NULL;
		return TRUE;
	}
	if (first_node != NULL){
		free(first_node);
		first_node = NULL;
	}
	return FALSE;
}

/* Função de criar e iniciar nó */
boolean function_firstItemPreparation(NODE *node){
	node->item_count = EMPTY;
	node->item = (ITEM **) malloc(sizeof(ITEM *) * node->node_size);
	if (node->item != NULL){
		for(int i = EMPTY; i < node->node_size; i++)
			node->item[i] = NULL;
		return TRUE;
	}
	return FALSE;
}

/* Esta função, de fato, coloca o nó alocado e inicializado com 
 function_nodeAutocreate() e o coloca na estrutura de Arraylist */
boolean function_nodeCreation(ARRAY_STRUCT *array_struct){
	NODE *node = function_nodeAutocreate();
	if (array_struct != NULL && node != NULL){
		if (arraylist_checkEmpty(array_struct)){
			free(node);
			return function_firstnodeCreation(array_struct);
		}
		node->node_size = (2 * (array_struct->node_end->node_size));
		node->node_start = (array_struct->node_end->node_size + array_struct->node_end->node_start);

		node->next = NULL;

		array_struct->node_end->next = node;
		array_struct->node_end = node;
		(array_struct->node_count++);
		return TRUE;
	}
	if (node != NULL){
		free(node);
		node = NULL;
	}
	return FALSE;
}

/* Esta aqui encontra um nó específico dado uma posição, porém força a "existência" de nós, 
dada uma posição fora dos limites atuais da arraylist*/
NODE *function_getnforceNode(ARRAY_STRUCT *array_struct, int pos){
	NODE *node = NULL;
	if (array_struct != NULL){
		node = array_struct->node_start;
		if (node == NULL){
			function_firstnodeCreation(array_struct);
			node = array_struct->node_start;
			if (pos < (node->node_size + node->node_start))
				return node;
		}

		while(node != NULL && pos >= (node->node_size + node->node_start)){
			node = node->next;
			if (node == NULL){
				function_nodeCreation(array_struct);
				node = array_struct->node_end;
				break;
			}

		}
	}
	return node;
}

/* Função de destruir um dado nó */
boolean function_nodeDestroy(NODE *node){
	if (node != NULL){
		if (node->item != NULL){
			for(int i = EMPTY; i < node->node_size; i++)
				if (node->item[i] != NULL){
					free(node->item[i]);
					node->item[i] = NULL;
				}
			free(node->item);
			}
		free(node);
		return TRUE;
	}
	return FALSE;
}

/* Esta função destrói a Arraylist e desaloca todos os possíveis nodes existentes */
boolean arraylist_destroyStructure(ARRAY_STRUCT **array_struct){
	NODE *aux_destroy = NULL, *node_next;
	if (array_struct != NULL && *array_struct != NULL){
		aux_destroy = (*array_struct)->node_start;
		while(aux_destroy != NULL){
			node_next = aux_destroy->next;
			function_nodeDestroy(aux_destroy);
			aux_destroy = node_next;
		}
		free(*array_struct);
		(*array_struct) = NULL;
		return TRUE;
	}
	return FALSE;
}

/* Função que verifica se a Arraylist está ou não vazia */
boolean arraylist_checkEmpty(ARRAY_STRUCT *array_struct){
	if (array_struct != NULL)
		if (array_struct->node_start != NULL)
			return FALSE;
	return TRUE;
}

/* Esta aqui conta quantos elementos ao total existem na arraylist */
int arraylist_checkSize(ARRAY_STRUCT *array_struct){
	int size = EMPTY;
	NODE *aux = NULL;
	if (array_struct != NULL){
		aux = array_struct->node_start;
		if (aux != NULL){
			size = ARRAY_INITIAL_SIZE;
			aux = aux->next;
			while (aux != NULL){
				size += aux->item_count;
				aux = aux->next;
			}
		}
	}
	return size;
}

/* Função para imprimir os itens com suas chaves correspondentes */
boolean arraylist_dumpData(ARRAY_STRUCT *array_struct){
	NODE *aux = NULL;
	if (array_struct != NULL){
		aux = array_struct->node_start;
		while(aux != NULL){
			if(aux->item != NULL){
				for(int i = EMPTY; i < aux->node_size; i++)
					if(aux->item[i] != NULL)
						printf("%d/%d\n", aux->item[i]->chave, aux->item[i]->valor);
				printf("\n\n");
			}
			aux = aux->next;
		}
		return TRUE;
	}
	return FALSE;
}

/* Esta função verifica se atualmente a arraylist está cheia, usada para acrescentar um nó adicional */
boolean arraylist_checkFull(ARRAY_STRUCT *array_struct){
	if (array_struct != NULL)
		if (arraylist_checkEmpty(array_struct) || array_struct->node_end->item_count == array_struct->node_end->node_size)
			return TRUE;
	return FALSE;
}

/* Esta função shifta a arraylist inteira uma posição para a direita*/
boolean arraylist_shiftRight(ARRAY_STRUCT *array_struct, int pos){
	NODE *node = NULL, *node_aux = function_getNode(array_struct, pos);
	int index = INVALID;
	if (array_struct != NULL && node_aux != NULL){
		node = array_struct->node_end;
		if(node != NULL){
			if (arraylist_checkFull(array_struct)){
				function_nodeCreation(array_struct);
				function_firstItemPreparation(array_struct->node_end);
				array_struct->node_end->item[EMPTY] = node->item[node->node_size - UNITY];
			}
			index = (node->node_size - UNITY);
			while(node != node_aux){
				for(int i = index; i > EMPTY; i--)
					node->item[i] = node->item[i - UNITY];
				node = function_getNode(array_struct, node->node_start - UNITY);
				if (node != NULL){
					index = (node->node_size - UNITY);
					node->next->item[EMPTY] = node->item[index];
				}
			}
			int i;
			for(i = (node_aux->node_start + node_aux->node_size - UNITY); i > pos; i--)
				node_aux->item[i] = node_aux->item[i - UNITY];
			node_aux->item[i + UNITY] = NULL;
			return TRUE;
		}
	}
	return FALSE;
}

/* Esta função shifta a arraylist inteira uma posição para a esquerda*/
boolean arraylist_shiftLeft(ARRAY_STRUCT *array_struct, int pos){
	NODE *node = NULL;
	int aux, i;
	if (array_struct != NULL){
		node = function_getNode(array_struct, pos);
		aux = (pos - node->node_start);
		if (node != NULL){
			while(node != NULL){
				for(i = aux; i < (node->node_size - UNITY); i++)
					node->item[i] = node->item[i + UNITY];
				if (i == (node->node_size - UNITY) && node->next != NULL){
					node->item[i] = node->next->item[EMPTY];
				} else node->item[i] = NULL;
				node = node->next;
				aux = EMPTY;
			}
			return TRUE;
		}
	}
	return FALSE;
}

/* Usei Insertion sort com tratamento para vários situações da arraylist para inserir os elementos de forma ordenada */
boolean function_insertionSort(ARRAY_STRUCT *array_struct, ITEM *item){
	NODE *node = NULL, *aux_node = NULL;
	boolean FLAG = TRUE, FOUNDED = FALSE;
	int i;
	if (array_struct != NULL && item != NULL){
		node = array_struct->node_end;
		/*Este flag funciona ate encontrarmos uma posicao ideal para o item */
		while(FLAG){
			for(i = (node->item_count - UNITY); i >= EMPTY; i--){
				if(node->item[i]->chave > item->chave)
					node->item[i + UNITY] = node->item[i];
				else {
					FOUNDED = TRUE;
					break;
				}
			}
			//Ha varios casos a serem considerados neste algoritmo junto a esta estrutura de arrays.
			if (node == array_struct->node_start || FOUNDED){
				node->item[i + UNITY] = item;
				FLAG = FALSE;
			}else{
				/*Quando entramos este curly brackets, sabemos que ou o item nao pertence ao node atual 
				ou ele pertence a primeira posicao do node atual */
				aux_node = function_getNode(array_struct, node->node_start - UNITY);
				if (aux_node->item[aux_node->node_size - UNITY]->chave < item->chave){
					node->item[i + UNITY] = item;
					FLAG = FALSE;
				}else {
					node->item[EMPTY] = aux_node->item[aux_node->node_size - UNITY];
					node = aux_node;
				}
			}
		}
		array_struct->node_end->item_count++;
		return TRUE;
	}
	return FALSE;
}

/* Função usada para adicionar um novo elemento de forma ordenada */
boolean arraylist_addElement(ARRAY_STRUCT *array_struct, COMMAND_STRUCT *command_struct){
	ITEM *item = NULL;
	int counter = EMPTY;

	if (array_struct != NULL && command_struct != NULL){
		if (arraylist_checkFull(array_struct)){
			function_nodeCreation(array_struct);
			function_firstItemPreparation(array_struct->node_end);
		}

		item = function_itemAutocreate();
		item->chave = command_struct->param[0];
		item->valor = command_struct->param[1];

		command_clearParameters(command_struct);
		return function_insertionSort(array_struct, item);
	}
	if (item != NULL){
		free(item);
		item = NULL;
	}
	return FALSE;
}

/* Esta função obtém um item especifico através de sua chave e retorna seu endereço */
ITEM *function_getItem(ARRAY_STRUCT *array_struct, COMMAND_STRUCT *command_struct){
	ITEM *item = NULL;
	NODE *node = NULL;
	int array_index = EMPTY;
	if (command_struct != NULL && array_struct != NULL){
		node = array_struct->node_start;
		while (node != NULL){
			if (node->item != NULL)
				item = function_binSearch(node->item, &array_index, node->item_count, command_struct->param[0]);
			if (item == NULL){
				node = node->next;
				item = NULL;
			} else break;
		}
	}
	return item;
}

/*Esta função retorna um item dado um determinado index na arraylist */
ITEM *function_getIteminIndex(ARRAY_STRUCT *array_struct, int pos){
	NODE *aux = NULL;
	if (array_struct != NULL && pos >= EMPTY){
		aux = function_getNode(array_struct, pos + UNITY);
		if (aux != NULL && aux->item != NULL && (pos - aux->node_start) >= EMPTY)
			return aux->item[pos - aux->node_start];
	}
	return NULL;
}

/* E esta verifica a existência de um item, mas não retorna seu endereço */
boolean arraylist_checkItem(ARRAY_STRUCT *array_struct, COMMAND_STRUCT *command_struct){
	if (command_struct != NULL && array_struct != NULL)
		if(function_getItem(array_struct, command_struct) != NULL)
			return TRUE;
	return FALSE;
}

/* Já esta retorna o index atual do item  na arraylist */
boolean arraylist_getIndex(ARRAY_STRUCT *array_struct, COMMAND_STRUCT *command_struct){
	ITEM *item = NULL;
	NODE *node = NULL;
	int array_index = INVALID;

	if (command_struct != NULL && array_struct != NULL){
		node = function_getNode(array_struct, command_struct->param[0]);
		if (node != NULL)
			item = function_binSearch(node->item, &array_index, node->node_size, command_struct->param[0]);
			if (item != NULL){
				printf("%d\n", node->node_start + array_index);
				return TRUE;
			}
	}
	return FALSE;
}

/* Esta função altera os valores de chave e valor do item. Para manter a arraylist ordenada, eu primeiro removo o item original da lista, 
e, depois, faço a inserção ordenada de sua cópia na arraylist. */
boolean arraylist_setValues(ARRAY_STRUCT *array_struct, COMMAND_STRUCT *command_struct){
	if (command_struct != NULL &&command_struct->param != NULL && array_struct != NULL){
		if (command_struct->param[0] <= (array_struct->node_end->node_start + array_struct->node_end->node_size)){
			arraylist_removeItem(array_struct, command_struct);
			command_struct->param[0] = command_struct->param[1];
			command_struct->param[1] = command_struct->param[2];
			return arraylist_addElement(array_struct, command_struct);
		}
	} 
	return FALSE;
}

/* E esta aqui cria uma sub-arraylist e copia alguns elementos da arraylist original, e a retorna */
ARRAY_STRUCT *arraylist_getSubvector(ARRAY_STRUCT *array_struct, COMMAND_STRUCT *command_struct){
	ARRAY_STRUCT *new_arraylist = NULL;
	ITEM *item = NULL;
	int lim_a, lim_b;

	if (array_struct != NULL && command_struct != NULL && command_struct->param != NULL && command_struct->param[1] > command_struct->param[0]){
		new_arraylist = arraylist_createStructure();
		lim_a = command_struct->param[0];
		lim_b = command_struct->param[1];
		for(int i = lim_a; i < lim_b; i++){
			item = function_getIteminIndex(array_struct, i);
			if (item != NULL){
				if (command_struct->param == NULL)
					command_struct->param = (int *) malloc(sizeof(int) * 2);
				if (command_struct->param != NULL){
					command_struct->param[0] = item->chave;
					command_struct->param[1] = item->valor;
					arraylist_addElement(new_arraylist, command_struct);
				}
			}
			item = NULL;
		}
	}
	return new_arraylist;
}

/* Esta função remove um item e shifta a arraylist para cobrir o "buraco" deixado */
boolean arraylist_removeItem(ARRAY_STRUCT *array_struct, COMMAND_STRUCT *command_struct){
	ITEM *item = NULL;
	NODE *node = NULL;
	if (command_struct != NULL && array_struct != NULL && command_struct->param[0] >= EMPTY){
		item = function_getIteminIndex(array_struct, command_struct->param[0]);
		if (item != NULL){
			free(item);
			arraylist_shiftLeft(array_struct, command_struct->param[0]);
			array_struct->node_end->item_count--;
			//Desaloca a memoria de nodes vazios.
			if (array_struct->node_end->item_count == EMPTY){
				node = function_getNode(array_struct, array_struct->node_end->node_start - UNITY);
				free(array_struct->node_end);
				array_struct->node_end = node;
			}
			return TRUE;
		}
	} 
	return FALSE;
}