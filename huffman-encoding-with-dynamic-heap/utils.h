#ifndef __UTILS_H_
#define __UTILS_H_

//Some useful stuff.

#define SWAP_VEC(A,B) {void *p = A; A = B; B = p;}
#define SWAP_INT(I,J) {int aux = I; I = J; J = aux;}
#define BITS_IN_BYTE 8

typedef unsigned char byte;

typedef enum{
	FALSE,
	TRUE
} bool;

enum{
	INVALID = -1,
	EMPTY,
	UNITY
};

#endif