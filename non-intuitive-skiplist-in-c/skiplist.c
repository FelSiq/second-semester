#include <stdlib.h>
#include <stdio.h>
#include "skiplist.h"
#include <time.h>
#include <string.h>

//#define DEBUG

struct NODE{
	ITEM *item;
	NODE *next, *upper;
};

struct SKIPLIST{
	NODE **node_heads;
	unsigned int level_num;
};

char *function_getNodeIp(NODE *node){
	if (node != NULL)
		return function_getIp(node->item);
	return NULL;
}

NODE *function_createNode(ITEM *item, NODE *next, NODE *upper){
	NODE *node = (NODE *) malloc(sizeof(NODE));
	if (node != NULL){
		node->item = item;
		node->upper = upper;
		node->next = (next == NULL) ? node : next;
	}
	return node;
}

boolean skiplist_createLevel(SKIPLIST *skiplist){
	#ifdef DEBUG
		printf("D: Trying to create a new headnode...\n");
	#endif
	if (skiplist != NULL){
		(skiplist->level_num)++;
		skiplist->node_heads = (NODE **) realloc(skiplist->node_heads, sizeof(NODE *) * (skiplist->level_num));
		if (skiplist->node_heads != NULL){
			skiplist->node_heads[skiplist->level_num - UNITY] = (NODE *) malloc(sizeof(NODE));
			if (skiplist->node_heads[skiplist->level_num - UNITY] != NULL){
				skiplist->node_heads[skiplist->level_num - UNITY]->item = NULL;
				skiplist->node_heads[skiplist->level_num - UNITY]->next = skiplist->node_heads[skiplist->level_num - UNITY];
				skiplist->node_heads[skiplist->level_num - UNITY]->upper = NULL;
				if ((int) skiplist->level_num - 2 >= EMPTY)
					skiplist->node_heads[skiplist->level_num - UNITY]->upper = skiplist->node_heads[skiplist->level_num - 2];
				return TRUE;
			}
			free(skiplist->node_heads);
		}
		(skiplist->level_num)--;
	}
	return FALSE;
}

SKIPLIST *skiplist_createStruct(){
	SKIPLIST *skiplist = (SKIPLIST *) malloc(sizeof(SKIPLIST));
	if (skiplist != NULL){
		skiplist->level_num = EMPTY;
		skiplist->node_heads = NULL;
		if (skiplist_createLevel(skiplist))
				return skiplist;
		free(skiplist);
	}
	return NULL;
}

NODE *function_copyNode(NODE *node){
	return function_createNode(node->item, NULL, node);
}

NODE *function_posSearch(NODE *node, char *key){
	if (node != NULL && key != NULL){
		if (node->next->item != NULL){
			int aux = strcmp(function_getHost(node->next->item), key);
			if (aux == EMPTY)
				return node->next;
			if (aux < EMPTY)
				return function_posSearch(node->next, key);
			if (node->upper != NULL)
				return function_posSearch(node->upper, key);
		}
		if (node->upper!= NULL)
			return function_posSearch(node->upper, key);
	}
	return node;
}

boolean function_sortedInsertion(NODE *node_head, NODE *new_node){
	if (node_head != NULL && new_node != NULL){
		NODE *aux = node_head;

		if(aux->next == aux){
			aux->next = new_node;
			new_node->next = aux;
			return TRUE;
		}
		while(aux->next != node_head && strcmp(function_getHost(aux->next->item), function_getHost(new_node->item)) < EMPTY)
			aux = aux->next;
		new_node->next = aux->next;
		aux->next = new_node;

		return TRUE;
	}
	return FALSE;
}

boolean skiplist_insert(SKIPLIST *skiplist, COMMANDS *commands){
	NODE *new_node = NULL, *copy_node = NULL;
	ITEM *item = NULL;
	unsigned int level_counter = EMPTY;
	boolean random = UNITY;
	clock_t clk;

	if (skiplist != NULL && commands != NULL){
		srand(time(NULL));
		item = function_createItem(function_getArgs(commands, HOST), function_getArgs(commands, IP));
		new_node = function_createNode(item, NULL, NULL);
		copy_node = new_node;
		while(random){
			clk = (clock()/((CLOCKS_PER_SEC/1000) * 1.0) * 100);
			srand(time(NULL) + clk);
			level_counter++;
			if(level_counter != UNITY)
				copy_node = function_copyNode(new_node);
			if(level_counter > skiplist->level_num)
				skiplist_createLevel(skiplist);
			function_sortedInsertion(skiplist->node_heads[level_counter - UNITY], copy_node);
			random = (rand() % 2 == EMPTY) ? UNITY : EMPTY;
			#ifdef DEBUG
				printf("D: clk value: %ld, random number value: %d, Node got promoted to level %d!\n", clk, random, level_counter - UNITY);
			#endif
		}
		return TRUE;
	}
	return FALSE;
}

boolean skiplist_remove(SKIPLIST *skiplist, COMMANDS *commands){
	if (skiplist != NULL && commands != NULL){
		//To be implemented...	
		return TRUE;
	}
	return FALSE;
}

NODE *skiplist_recursiveSearch(NODE *node, char *key){
	if (node != NULL && key != NULL){
		if (node->next->item != NULL){
			int aux = strcmp(function_getHost(node->next->item), key);
			if (aux == EMPTY)
				return node->next;
			if (aux < EMPTY)
				return skiplist_recursiveSearch(node->next, key);
			if (node->upper != NULL)
				return skiplist_recursiveSearch(node->upper, key);
		}
		return skiplist_recursiveSearch(node->upper, key);
	}
	return NULL;
}

NODE *skiplist_search(SKIPLIST *skiplist, COMMANDS *commands){
	if (skiplist != NULL && skiplist->node_heads != NULL && commands != NULL)
		return skiplist_recursiveSearch(skiplist->node_heads[skiplist->level_num - UNITY], function_getArgs(commands, HOST));
	return NULL;
}

NODE *skiplist_seqSearch(SKIPLIST *skiplist, COMMANDS *commands){
	if (skiplist != NULL && skiplist->node_heads != NULL && commands != NULL){
		NODE *aux = skiplist->node_heads[EMPTY];
		if (aux != NULL){
			aux = aux->next;
			while(aux != skiplist->node_heads[EMPTY] && strcmp(function_getHost(aux->item), function_getArgs(commands, HOST)))
				aux = aux->next;
			if (aux != skiplist->node_heads[EMPTY])
				return aux;
		}
	}
	return NULL;
}

boolean skiplist_mapnodes(SKIPLIST *skiplist){
	if (skiplist != NULL && skiplist->node_heads != NULL){
		NODE *aux = NULL;
		for(int i = EMPTY; i < skiplist->level_num; i++){
			aux = skiplist->node_heads[i]->next;
			while(aux != skiplist->node_heads[i]){
				printf("%s ", function_getIp(aux->item));
				aux = aux->next;
			}
			printf("\n");
		}
		return TRUE;
	}
	return FALSE;
}

boolean skiplist_destroy(SKIPLIST **skiplist){
	if (skiplist != NULL && *skiplist != NULL){
		NODE *aux = NULL, *aux_remove = NULL;
		if ((*skiplist)->node_heads != NULL){
			while((*skiplist)->level_num > EMPTY){
				aux = (*skiplist)->node_heads[(*skiplist)->level_num - UNITY]->next;
				while(aux != (*skiplist)->node_heads[(*skiplist)->level_num - UNITY]){
					aux_remove = aux;
					aux = aux->next;
					if (((*skiplist)->level_num - UNITY == EMPTY) && aux_remove->item != NULL){
						function_itemDestroy(&aux_remove->item);
						aux_remove->item = NULL;
					}
					free(aux_remove);
				}
				free(aux);
				(*skiplist)->node_heads[(*skiplist)->level_num - UNITY] = NULL;
				((*skiplist)->level_num)--;
			}
			free((*skiplist)->node_heads);
			(*skiplist)->node_heads = NULL;
		}
		free(*skiplist);
		(*skiplist) = NULL;
		return TRUE;
	}
	return FALSE;
}