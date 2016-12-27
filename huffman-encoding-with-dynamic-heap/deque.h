#ifndef __DEQUE_H_
#define __DEQUE_H_
#include "dynheap.h"

typedef struct deque DEQUE;

DEQUE *deque_init();
NODE *deque_pop_end(DEQUE *);
NODE *deque_pop_start(DEQUE *);
bool deque_empty(DEQUE *);
void deque_push_end(DEQUE *, NODE *);
void deque_push_start(DEQUE *, NODE *);
void deque_destroy(DEQUE *);

#endif