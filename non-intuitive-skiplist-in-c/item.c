#include <stdlib.h>
#include <stdio.h>
#include "item.h"

struct ITEM{
	char *ip, *host;
};

ITEM *function_createItem(char *str_host, char *str_ip){
	ITEM *item = (ITEM *) malloc(sizeof(ITEM));
	if (item != NULL){
		item->host = str_host;
		item->ip = str_ip;
	}
	return item;
}

char *function_getIp(ITEM *item){
	if (item != NULL)
		return item->ip;
	return NULL;
}

char *function_getHost(ITEM *item){
	if (item != NULL)
		return item->host;
	return NULL;
}

boolean function_itemDestroy(ITEM **item){
	if(item != NULL && *item != NULL){
		if ((*item)->ip != NULL){
			free((*item)->ip);
			(*item)->ip = NULL;
		}
		if((*item)->host != NULL){
			free((*item)->host);
			(*item)->host = NULL;
		}
		free(*item);
		(*item) = NULL;
		return TRUE;
	}
	return FALSE;
}