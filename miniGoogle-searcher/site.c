#include <stdlib.h>
#include <stdio.h>
#include "googlebot.h"


SITE *function_SiteAutocomplete(){
	SITE *new_site = (SITE *) malloc(sizeof(SITE));
	if (new_site != NULL){
		new_site->site_code = INVALID; 
		new_site->site_relevance = EMPTY;
		new_site->site_kwnum = EMPTY;
		new_site->site_keywords = NULL; 
		new_site->site_name = NULL; 
		new_site->site_homelink = NULL;
		new_site->site_next = NULL;
		new_site->site_prev = NULL;
	}
	return new_site;
}

SITE *function_SiteGet(GOOGLEBOT_STRUCT *googlebot, short int key){
	SITE *aux = NULL;
	if (googlebot != NULL && key >= EMPTY){
		googlebot->googlebot_head->site_code = key;
		aux = googlebot->googlebot_head->site_next;
		while(key != aux->site_code)
			aux = aux->site_next;
		googlebot->googlebot_head->site_code = INVALID;
		if (aux != googlebot->googlebot_head)
			return aux;
	}
	return NULL;
}

boolean function_SiteDestroy(SITE *site){
	if (site != NULL){
		if (site->site_keywords != NULL)
			matrix_destroy(site->site_keywords, site->site_kwnum);
		if (site->site_name != NULL)
			free(site->site_name);
		if (site->site_homelink != NULL)
			free(site->site_homelink);
		free(site);
		return TRUE;
	}
	return FALSE;
}

boolean function_SiteAutoinsertion(GOOGLEBOT_STRUCT *googlebot, SITE *new_site){
	SITE *site_aux = NULL;
	if (googlebot != NULL && new_site != NULL){
		googlebot->googlebot_head->site_code = (new_site->site_code + UNITY);
		site_aux = googlebot->googlebot_head->site_next;

		while (new_site->site_code > site_aux->site_code)
			site_aux = site_aux->site_next;
		if (site_aux != NULL){
			new_site->site_next = site_aux;
			new_site->site_prev = site_aux->site_prev;
			site_aux->site_prev->site_next = new_site;
			site_aux->site_prev = new_site;

			googlebot->googlebot_head->site_code = INVALID;
			return TRUE;
		}
	}
	return FALSE;
}

void merge_ordenate(SITE **vector, short int start, short int middle, short int end){
	SITE **aux_vector = (SITE **) malloc(sizeof(SITE *) * (end - start + UNITY));
	short int i = start, j = (middle + UNITY), k = EMPTY;
	while(i <= middle || j < end){
		if (i > middle) aux_vector[k++] = vector[j++];
		else{
			if (j > end) aux_vector[k++] = vector[i++];
			else{
				if (vector[i]->site_relevance > vector[j]->site_relevance)
					aux_vector[k++] = vector[i++];
				else
					aux_vector[k++] = vector[j++];
			}
		}
	}

	for(i = EMPTY; i < k; i++)
		vector[start + i] = aux_vector[i];

	free(aux_vector);
}

void merge_sort(SITE **vector, short int start, short int end){
	if (end != start){
		short int middle = (start + end)/2;
		merge_sort(vector, start, middle);
		merge_sort(vector, middle + UNITY, end);
		merge_ordenate(vector, start, middle, end);
	}
}