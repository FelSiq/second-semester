#ifndef __HUFFMAN_T_
#define __HUFFMAN_T_
#define ARG_NUM 4
#define ASCII_SIZE 128
#define SWAP_ADRESS(A,B) {void *C = A; A = B; B = C;}
#define MAX_SYMBOL_SHIFT (1 << ((sizeof(unsigned short int) * 8) - 1))
#define MAX_MASK_SHIFT (1 << ((sizeof(unsigned char) * 8) - 1))
#define MAX_BYTE_VAL 255
#define MAX_LBYTE_VAL 65535
#define BITS_IN_BYTE 8

#include "utils.h"

enum{
	ARG_PROGNAME,
	ARG_FINPUT,
	ARG_FOUTPUT,
	ARG_OPERATION
};

enum{
	DO_COMPRESS,
	DO_DECOMPRESS
};

typedef struct huffman_t HUFFMAN_T;
typedef struct node NODE;
typedef unsigned char byte;
typedef unsigned short int l_byte;

HUFFMAN_T *huffmanInit();
void huffmanCompress(HUFFMAN_T *, FILE *const, FILE *);
void huffmanDecompress(HUFFMAN_T *, FILE *const, FILE *);
boolean huffmanDestroy(HUFFMAN_T **);

#endif