#ifndef __UTILS_H_
#define __UTILS_H_

#define BASE_DEC 10
#define ASCII0 48
#define SWAP_I(A,B) {int aux = A; A = B; B = aux;}
#define SWAP_F(A,B) {float aux = A; A = B; B = aux;}
#define SWAP_D(A,B) {double aux = A; A = B; B = aux;}
#define SWAP_V(A,B) {void *p = A; A = B; B = p;}
#define MAX_INT ((1 << sizeof(int)*8) - 1)
#define MAX(A,B) (A > B) ? A : B
#define MIN(A,B) (A < B) ? A : B

typedef enum{
	FALSE,
	TRUE
} bool;

enum{
	INVALID = -1,
	EMPTY,
	UNITY
};

enum{
	ENTER = 10,
	CR = 13,
	SPACEBAR = 32
};

#endif