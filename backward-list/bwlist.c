#include <stdlib.h>
#include <stdio.h>
#include "bwlist.h"

boolean entries_get(char *operator, int *key, int *back_index){
	if (operator != NULL)
		scanf("%c", operator);
	switch(*operator){
		case OP_INSERT:
			scanf("%d%d", key, back_index);
			getchar();
			return TRUE;
		case OP_REMOVE:
			scanf("%d", key);
			getchar();
			return TRUE;
		case OP_END:
			getchar();
			return TRUE;
	}
	return FALSE;
}

boolean entries_clean(char *operator, int *key, int *back_index){
	if (operator != NULL)
		(*operator) = EMPTY;
	if (key != NULL)
		(*key) = EMPTY;
	if (back_index != NULL)
		(*back_index) = INVALID;
	return FALSE;
}

LIST_STRUCT *listStruct_create(){
	LIST_STRUCT *list_struct = (LIST_STRUCT *) malloc(sizeof(LIST_STRUCT));
	if (list_struct != NULL){
		list_struct->start = NULL;
		list_struct->end = NULL;
		list_struct->time = EMPTY;
	}
	return list_struct;
}

boolean listStruct_destroy(LIST_STRUCT **list_struct){
	NODE *aux_destroy = NULL, *aux_next = NULL;
	if(list_struct != NULL && *list_struct == NULL){
		aux_destroy = (*list_struct)->start;
		while(aux_destroy != NULL){
			aux_next = aux_destroy->next;
			free(aux_destroy);
			aux_destroy = aux_next;
		}
		free(*list_struct);
		(*list_struct) = NULL;
		return TRUE;
	}
	return FALSE;
}

void function_cleanReference(LIST_STRUCT *list_struct, NODE *adress){
	NODE *aux = NULL;
	if (list_struct != NULL && adress != NULL && list_struct->start != NULL){
		aux = adress->next;
		while(aux != NULL){
			if(aux->back == adress)
				aux->back = NULL;
			aux = aux->next;
		}
	}
}

boolean listStruct_remove(LIST_STRUCT *list_struct, int key){
	NODE *aux_remove = NULL;
	if(list_struct != NULL){
		aux_remove = list_struct->start;
		/* CASE REMOVING FIRST NODE*/
		if (aux_remove != NULL && aux_remove->key == key){
			list_struct->start = aux_remove->next;
			function_cleanReference(list_struct, aux_remove);
			free(aux_remove);
			aux_remove = NULL;

			(list_struct->time)++;
			return TRUE;
		}
		/*FOR OTHERS NODES*/
		while(aux_remove->next != NULL && aux_remove->next->key != key)
			aux_remove = aux_remove->next;
		if (aux_remove->next != NULL){
			NODE *aux_next = aux_remove->next->next;
			if (aux_remove->next == list_struct->end)
				list_struct->end = aux_remove;
			function_cleanReference(list_struct, aux_remove->next);
			free(aux_remove->next);
			aux_remove->next = aux_next;
			aux_next = NULL;

			(list_struct->time)++;
			return TRUE;
		}
	}
	return FALSE;
}

NODE *node_create(){
	NODE *node = (NODE *) malloc(sizeof(NODE));
	if (node != NULL){
		node->key = EMPTY;
		node->time = EMPTY;
		node->next = NULL;
		node->back = NULL;
	}
	return node;
}
/* Essa função encontra o nó "back" correto recursivamente" */
void get_position(NODE *position, NODE *adress, int *back_index){
	if (position == NULL){
		(*back_index)--;
		return;
	}

	get_position(position->next, adress, back_index);

	if ((*back_index) == EMPTY){
		adress->back = position;
		(*back_index)--;
		return;
	}

	(*back_index)--;
}

boolean listStruct_add(LIST_STRUCT *list_struct, int key, int back_index){
	NODE *node = NULL;
	if (list_struct != NULL){
		node = node_create();
		if (node != NULL){
			if (list_struct->start == NULL)
				list_struct->start = node;
			node->time = list_struct->time;
			node->key = key;

			if (back_index != EMPTY)
				get_position(list_struct->start, node, &back_index);

			if (list_struct->end != NULL)
				list_struct->end->next = node;
			list_struct->end = node;

			(list_struct->time)++;
			return TRUE;
		}
	}
	return FALSE;
}

boolean listStruct_print(LIST_STRUCT *list_struct){
	NODE *aux = NULL, *aux_counter = NULL;
	int counter = EMPTY;
	if(list_struct != NULL){
		if (list_struct->start != NULL){
			aux = list_struct->start;
			while(aux != NULL){
				if (aux->back != NULL){
					aux_counter = list_struct->start;
					while(aux_counter != aux->back){
						aux_counter = aux_counter->next;
						counter++;
					}
					printf("[%d,%d,%d] ", aux->key, aux->time, counter);
					counter = EMPTY;
				}else printf("[%d,%d] ", aux->key, aux->time);
				aux = aux->next;
			}
		} else printf("%d", INVALID);
		printf("\n");
		return TRUE;
	}
	return FALSE;
}