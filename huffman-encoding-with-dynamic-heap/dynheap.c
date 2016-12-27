#include <stdlib.h>
#include <stdio.h>
#include "deque.h"

struct node{
	struct node *son_l, *son_r;
	int key, num_sons;
	ITEM *item;
};

struct heapd{
	NODE *root;
	//This memory is used to know where to insert the new node;
	//Please note that the deque DOES NOT replace the dynamic heap methods by any means.
	DEQUE *memory; 
	int num_sons;
};

//Inits the heap and the insertion memory deque
HEAPD *heapd_init(){
	HEAPD *heapd = (HEAPD *) malloc(sizeof(HEAPD));
	if (heapd != NULL){
		heapd->root = NULL;
		heapd->num_sons = EMPTY;
		heapd->memory = deque_init();
		if (heapd->memory != NULL)
			return heapd;
		free(heapd);
	}
	return NULL;
};

/*There's four functions to add a node:

	(start) heapd_insert (complexity theta(1)): Creates node and init it
				||
				\/
	function_auxInsert (complexity theta(1)): insert the new node using a auxiliary deque.
				||
				\/
	function_fixheap (complexity theta(lg(n))): Prepare things up to fix-up.
				||
				\/
	(end) function_fixup [recursive] (complexity theta(lg(n))): keep dynamic heap a min heap.

	Total insertion complexity: 2*[theta(lg(n)) + theta(1)] ≃ Big-Oh(lg(n));

*/


//Keeps the heap a min heap. This function is theta(lg(n)).
static void function_fixup(NODE *node, int const code, int mask){
	if (node != NULL && mask >= UNITY){
		if (code & mask){
			function_fixup(node->son_l, code, mask >> UNITY);
			if (node->son_l != NULL && node->son_l->key < node->key)
				SWAP(node, node->son_l);
		} else {
			function_fixup(node->son_r, code, mask >> UNITY);
			if (node->son_r != NULL && node->son_r->key < node->key)
				SWAP(node, node->son_r);
		}
		node->num_sons++;
	}
};

//Prepare things to fix up the heap to keep it a min heap. This functions theta(lg(n)), because while
//calculates all the way of a given index to the elder root of dynamic heap. Please note that this pre-
//paration is much faster than the fixup function, because it only does arithmetic stuff.
//The idea of this function is to create a bitmap to guide my through the dynamic heap, starting from the elder
//node til the newest, and the fix the way (if needed) to keep the heap a (in this case) min heap.
//The fixup is theta(lg(n)).
static void function_fixheap(HEAPD *const heapd){
	int code = EMPTY, mask = UNITY, index = (heapd->num_sons - UNITY);
	//First, I create a bitmap to show me the way to the new node.
	while(index > EMPTY){
		if (index & UNITY)
			code |= mask;
		mask <<= UNITY;
		index = FATHER(index);
	}
	//Then I get to the him recursively and fix the heap if needed
	function_fixup(heapd->root, code, mask >> UNITY);
}

//Insert the new node at the perfect exacly position on the heap to keep it as complete as possible,
//using a deque memory method. After it, call fixup to keep the dynamic heap a (in this case) min heap.
//This function is theta(1), because it does not travel on the heap.
static void function_auxInsert(HEAPD *const heapd, NODE *new_node){
	if (heapd != NULL){
		NODE *father = deque_pop_start(heapd->memory);
		//father always have a empty son.
		if (father->son_l == NULL){
			//left son is empty, so does right son;
			father->son_l = new_node;
			deque_push_start(heapd->memory, father);
		} else {
			//right son is empty;
			father->son_r = new_node;
		}

		deque_push_end(heapd->memory, new_node);
		function_fixheap(heapd);
	}
};

//Insert creates a new node and insert it to the dynamic heap.
bool heapd_insert(HEAPD *const heapd, int const key, ITEM *item){
	if (heapd != NULL){
		NODE *new_node = (NODE *) malloc(sizeof(NODE));
		new_node->key = key;
		new_node->son_l = NULL;
		new_node->son_r = NULL;
		new_node->item = item;
		new_node->num_sons = EMPTY;

		if (new_node != NULL){
			++heapd->num_sons;
			if (heapd->root != NULL)
				function_auxInsert(heapd, new_node);
			 else{
				heapd->root = new_node;
				deque_push_start(heapd->memory, new_node);
			}
			return TRUE;
		}
	}
	return FALSE;
};

//Remove a specif node on dynamic heap at given index. Index simulates a static heap to referece.
NODE *heapd_remove(HEAPD *const heapd, int const index){
	if (heapd != NULL && index >= EMPTY){
		NODE *node = heapd->root;
		if (node != NULL && index <= node->num_sons){
			if (node->num_sons != EMPTY){
				//The method to find the node is pretty much like huffman encoding.
				int code = EMPTY, mask = UNITY, index_b = FATHER(heapd->root->num_sons);
				NODE *rem = heapd_get(heapd, index), *aux_p = NULL, *major = NULL;
				while(index_b > EMPTY){
					if (index_b & UNITY)
						code |= mask;
					index_b = FATHER(index_b);
					mask <<= UNITY;
				}
				node->num_sons--;
				while(mask > UNITY){
					mask >>= UNITY;
					if (code & mask)
						node = node->son_l;
					else
						node = node->son_r;
					node->num_sons--;
				}
				//I've got the last node's father
				if(node->son_r != NULL){
					aux_p = node->son_r;
					node->son_r = NULL;
					//Push the element of the deque memory because he can have a son now.
					deque_push_start(heapd->memory, node);
				} else {
					aux_p = node->son_l;
					node->son_l = NULL;
				}
				node = aux_p;
				//Now node's the last node.
				SWAP(node, rem);
				//Removed. Now the fix-down...
				major = rem;
				while(rem != NULL && rem->son_l != NULL){
					if (rem->son_l->key < major->key)
						major = rem->son_l;
					if (rem->son_r != NULL && rem->son_r->key < major->key)
						major = rem->son_r;
					if (rem != major){
						SWAP(major, rem);
						rem = major;
					} else break;
				}
			} else {
				heapd->root = NULL;
			}
			//Finalization
			//Remove the last node on the deque memory because he does not exits on dyn heap anymore.
			deque_pop_end(heapd->memory);
			--heapd->num_sons;
			return node;
		}
	}
	return NULL;
};

//Recovers the item of the root node.
ITEM *heapd_root(HEAPD *const heapd){
	return (heapd == NULL) ? NULL : ((heapd->root == NULL) ? NULL : heapd->root->item);
};

//Recover the item of the last node.
ITEM *heapd_last(HEAPD *const heapd){
	if (heapd != NULL && heapd->root != NULL)
		return heapd_get(heapd, heapd->root->num_sons)->item;
	return NULL;
};

//Check if dynamic heap is empty.
bool heapd_empty(HEAPD *const heapd){
	if (heapd != NULL && heapd->root != NULL)
		return FALSE;
	return TRUE;
};

//Get the adress of a specific node. The index simulates a static heap to reference.
NODE *heapd_get(HEAPD *const heapd, int index){
	if (heapd != NULL && index >= EMPTY){
		NODE *node = heapd->root;
		if (node != NULL && index < heapd->num_sons){
			int code = EMPTY, mask = UNITY;
			
			while(index > EMPTY){
				if (index & UNITY)
					code |= mask;
				mask <<= UNITY;
				index = FATHER(index);
			}
			
			while(mask > UNITY){
				mask >>= UNITY;
				if (code & mask)
					node = node->son_l;
				else
					node = node->son_r;
			}
			return node;
		}
	}
	return NULL;
};

//Returns the number of nodes on dynamic heap.
int heapd_count(HEAPD *const heapd){
	return (heapd == NULL) ? INVALID : ((heapd->root == NULL) ? EMPTY : heapd->root->num_sons + UNITY);
};

//All of these print methods are not used on the huffman program, but I decided to include then because why not.
static void function_auxPrintNestled(NODE *const root){
	if (root->son_l != NULL){
		printf(", {%d", root->son_l->key);
		function_auxPrintNestled(root->son_l);
		printf("}");
	}
	if (root->son_r != NULL){
		printf(", {%d", root->son_r->key);
		function_auxPrintNestled(root->son_r);
		printf("}");
	}
};

bool heapd_printNestled(HEAPD *const heapd){
	if (heapd != NULL){
		printf("{");
		if (heapd->root != NULL){
			printf("%d", heapd->root->key);
			function_auxPrintNestled(heapd->root);
		}
		printf("}\n");
		return TRUE;
	}
	return FALSE;
};

static void function_auxPrintList(NODE *const root, short int level){
	if (root != NULL){
		for(short int aux = level; aux > EMPTY; aux--)
			printf(">");
		printf("%d\n", root->key);
		function_auxPrintList(root->son_l, level + UNITY);
		function_auxPrintList(root->son_r, level + UNITY);
	}
};

bool heapd_printList(HEAPD *const heapd){
	if (heapd != NULL){
		function_auxPrintList(heapd->root, EMPTY);
		return TRUE;
	}
	return FALSE;
};

static void function_auxDestroy(NODE *root){
	if (root != NULL){
		function_auxDestroy(root->son_l);
		function_auxDestroy(root->son_r);
		free(root);
	}
};

//Colapses the dynamic heap itself but does not undo the init() function
bool heapd_clear(HEAPD *heapd){
	if (heapd != NULL){
		function_auxDestroy(heapd->root);
		heapd->root = NULL;
		return TRUE;
	}
	return FALSE;
};

//Colapses the dynamic heap structure
bool heapd_destroy(HEAPD **heapd){
	if (heapd != NULL && *heapd != NULL){
		function_auxDestroy((*heapd)->root);
		if ((*heapd)->memory != NULL)
			deque_destroy((*heapd)->memory);
		free(*heapd);
		(*heapd) = NULL;
		return TRUE;
	}
	return FALSE;
};


//Some request methods. Could or not be used in huffman program, but i decided to include all of then because why not.
int node_key(NODE *const node){
	return (node == NULL) ? INVALID : node->key;
}

ITEM *node_item(NODE *const node){
	return (ITEM *) ((node == NULL) ? NULL : node->item);
};

ITEM *node_lson(NODE *const node){
	return (ITEM *) ((node == NULL) ? NULL : ((node->son_l == NULL) ? NULL : node->son_l->item));
};

ITEM *node_rson(NODE *const node){
	return (ITEM *) ((node == NULL) ? NULL : ((node->son_r == NULL) ? NULL : node->son_r->item));
};

int node_numson(NODE *const node){
	return (node == NULL) ? INVALID : node->num_sons;
};