#ifndef __SKIPLIST_
#define __SKIPLIST_

#include "item.h"
#include "utils.h"

typedef struct NODE NODE;
typedef struct SKIPLIST SKIPLIST;

SKIPLIST *skiplist_createStruct();
boolean skiplist_insert(SKIPLIST *, COMMANDS *);
boolean skiplist_remove(SKIPLIST *, COMMANDS *);
NODE *skiplist_search(SKIPLIST *, COMMANDS *);
NODE *skiplist_seqSearch(SKIPLIST *, COMMANDS *);
boolean skiplist_mapnodes(SKIPLIST *);
boolean skiplist_destroy(SKIPLIST **);
char *function_getNodeIp(NODE *);

#endif