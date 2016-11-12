#ifndef _SITE_H_
#define _SITE_H_

typedef struct site{
	short int site_code, site_relevance, site_kwnum;
	char **site_keywords, *site_name, *site_homelink;
	struct site *site_next, *site_prev;
} SITE;

#define MAX_KEYWORD_NUMBER 10
#define MAX_SITENAME_LENGHT 50
#define MAX_LINK_LENGHT 100
#define MAX_RELEVANCE 1000

SITE *function_SiteAutocomplete();
boolean function_SiteDestroy(SITE *);
void merge_sort(SITE **, short int, short int);

#endif