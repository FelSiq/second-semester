#ifndef ___HUFF_H_
#define ___HUFF_H_

#include "utils.h"
#include "dynheap.h"
#define ASCII_SIZE 128

typedef struct tabela_huffman TABELA_HUFFMAN; 

TABELA_HUFFMAN *criarTabelaHuffman();
int compactarHuffman(char *const, char **, TABELA_HUFFMAN **);
char *descompactarHuffman(char *const, int, TABELA_HUFFMAN *);
void liberarTabelaHuffman(TABELA_HUFFMAN **);

#endif