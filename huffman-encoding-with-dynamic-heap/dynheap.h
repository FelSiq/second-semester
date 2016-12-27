#ifndef __HEAP_DIM_
#define __HEAP_DIM_

#define FATHER(A) (A - 1)/2
#define ROOT 0

#include "utils.h"

#define SWAP(A,B) {SWAP_VEC(A->item, B->item); SWAP_INT(A->key, B->key);}

typedef struct s_item ITEM;
typedef struct node NODE;
typedef struct heapd HEAPD;

HEAPD *heapd_init();					// Inits a new empty dynamic heap. Return NULL if fail.
NODE *heapd_get(HEAPD *, int);		// Return the adress of the node at the given index in the given heap. Returns NULL if node does not exits or fail.
ITEM *heapd_root(HEAPD *);			// Request heap elder node (index 0). Return NULL if heap is empty or fail.
ITEM *heapd_last(HEAPD *);			// Request the newest (last) node. Return NULL if heap is empty or fail. 
NODE *heapd_remove(HEAPD *, int);	// Remove a node at the heap, if it exists, in the given index. Returns NULL if node does not exits or remove fails.
bool heapd_insert(HEAPD *, int, ITEM *);	// Insert a new node at the heap and automatically fix it, if necessary. Return FALSE if fail.
bool heapd_clear(HEAPD *);			// Delete all nodes on the heap. Return TRUE if success, else FALSE.
bool heapd_empty(HEAPD *);			// Return TRUE if heap is empty or fail, else FALSE.
bool heapd_printNestled(HEAPD *);	// Print all the elements of the heap in nestled notation. Returns TRUE if success.
bool heapd_printList(HEAPD *);		// Same as above, but in indented list notation. Returns TRUE if success.
bool heapd_destroy(HEAPD **);		// Destroy the heap. Returns TRUE if success or FALSE if fail.
int heapd_count(HEAPD *);			// Return the number of nodes, or -1 if fail.

int node_key(NODE *);				// Request node's key. Return -1 if fail.
int node_numson(NODE *);			// Request node's number of sons. Return -1 if fail.
ITEM *node_lson(NODE *);			// Request node's left son. Return NULL if fail.
ITEM *node_rson(NODE *);			// Request node's left son. Return NULL if fail.
ITEM *node_item(NODE *);			// Request node's item. Return NULL if fail.

#endif