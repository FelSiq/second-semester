#include <stdlib.h>
#include <stdio.h>
#include "huff.h"

struct s_item {
	struct s_item *son_l, *son_r;
	char symbol;
};

struct tabela_huffman{
	ITEM *huffman_t;
	char *symbols;
	short int *codes;
};

//Inits the huffman table
TABELA_HUFFMAN *criarTabelaHuffman(){
	TABELA_HUFFMAN *huff_table = (TABELA_HUFFMAN *) malloc(sizeof(TABELA_HUFFMAN));
	if (huff_table != NULL){
		huff_table->huffman_t = NULL;
		huff_table->symbols = (char *) malloc(sizeof(char) * (UNITY + ASCII_SIZE));
		if (huff_table->symbols != NULL){
			huff_table->codes = (short int *) malloc(sizeof(short int) * (ASCII_SIZE));
			if (huff_table->codes != NULL){
				for(short int c = EMPTY; c < ASCII_SIZE; 
					huff_table->codes[c] = EMPTY, huff_table->symbols[c] = (char) c, ++c);
				huff_table->symbols[ASCII_SIZE] = '\0';
				return huff_table;
			} else
				free(huff_table->symbols);
		} else
			free(huff_table);
	}
	return NULL;
};

//Creates the huffman table
static void create_table(ITEM *item, TABELA_HUFFMAN *huff_table, short int mask, short int code){
	if (item != NULL){
		create_table(item->son_l, huff_table, mask << UNITY, code);
		create_table(item->son_r, huff_table, mask << UNITY, code | mask);
		if (item->symbol != INVALID)
			huff_table->codes[(int) item->symbol] = code;
	}
};

//Destroy the huffman tree
static void huffman_destroy(ITEM *item){
	if (item != NULL){
		huffman_destroy(item->son_r);
		huffman_destroy(item->son_l);
		free(item);
	}
};

int compactarHuffman(char *const text, char **new_text, TABELA_HUFFMAN **huff_table){
	if (text != NULL && huff_table != NULL && *huff_table != NULL){
		//Auxiliary variables
		short int i, mask_code;
		ITEM *item, *traveller;
		NODE *node;
		byte mask_pack, counter = EMPTY;
		int freq;
		//Heapd used to calculate the compressed codes
		HEAPD *heapd = heapd_init();
		if (heapd != NULL){
			//This function must return the size of the compressed text
			int new_size = EMPTY;
			//This is a auxiliary vector to keep the frequency of the symbols before adding then to the dynamic heap
			short int *aux = (short int *) malloc(sizeof(short int) * ASCII_SIZE);
			if (aux != NULL){
				//cleaning things up...
				for (i = (ASCII_SIZE - UNITY); i >= EMPTY; --i)
					*(aux + i) = EMPTY;
				//Get the recurrences on text...
				for(i = EMPTY; text[i] != '\0'; ++i)
					aux[(int) text[i]]++;
				//Added then to the heap...
				for(i = ASCII_SIZE - UNITY; i >= EMPTY; i--)
					if (aux[i] > EMPTY){
						item = (ITEM *) malloc(sizeof(ITEM));
						item->son_r = NULL;
						item->son_l = NULL;
						item->symbol = i;
						heapd_insert(heapd, aux[i], item);
						item = NULL;
					}
				//Free the aux vector
				free(aux);
				//Now build-up huffman tree...
				while(heapd_count(heapd) > UNITY){
					freq = EMPTY;
					item = (ITEM *) malloc(sizeof(ITEM));
					item->symbol = INVALID;
					
					node = heapd_remove(heapd, ROOT);
					freq += node_key(node);
					item->son_l = node_item(node);
					free(node);

					node = heapd_remove(heapd, ROOT);
					freq += node_key(node);
					item->son_r = node_item(node);
					free(node);

					heapd_insert(heapd, freq, item);
				}

				node = heapd_remove(heapd, ROOT);
				(*huff_table)->huffman_t = node_item(node);
				free(node);

				//Now its time to generate the huffman_table
				create_table((*huff_table)->huffman_t, *huff_table, UNITY, EMPTY);
				//Now we generate the compacted text: 
				(*new_text) = (char *) malloc(sizeof(char));
				(*new_text)[EMPTY] = EMPTY;
				for(i = EMPTY, mask_code = UNITY, mask_pack = UNITY; text[i] != '\0' && text[i] != '\n'; mask_code = UNITY, i++){
					traveller = (*huff_table)->huffman_t;
					while(traveller->symbol != text[i]){
						if (((*huff_table)->codes[(int) text[i]] & mask_code) >= UNITY){
							traveller = traveller->son_r;
							(*new_text)[new_size] |= mask_pack;
						} else
							traveller = traveller->son_l;
						mask_pack <<= UNITY;
						mask_code <<= UNITY;
						if (mask_pack == EMPTY){
							(*new_text) = (char *) realloc(*new_text, sizeof(char) * ((new_size + 4)));
							(*new_text)[++new_size] = EMPTY;
							mask_pack = UNITY;
						}
					}
				}
				//Now the last character
				//If mask_code == 1, then the last character is full. counter must be fixed in 8.
				//Else, counter must be the times the mask can shift to the right
				if (mask_pack == UNITY)
					counter = BITS_IN_BYTE;
				else {
					while(mask_pack > UNITY){
						mask_pack >>= UNITY;
						counter++;
					}
				}
				//Add the number of bits used in last byte of encoded text
				(*new_text)[++new_size] = counter;
				//And the \0
				(*new_text)[++new_size] = '\0';
			}
			//Destroy the dynamic heap used to construct the huffman tree
			heapd_destroy(&heapd);
			return new_size;
		}
	}
	return INVALID; 
};

//Huffman decompression
char *descompactarHuffman(char *const text, int size, TABELA_HUFFMAN *huff_table){
	if (text != NULL && size > EMPTY && huff_table != NULL && huff_table->huffman_t != NULL){
		//Auxiliary variables
		byte mask_pack = UNITY;
		int new_size = EMPTY, i = EMPTY;
		char *new_text = NULL;
		ITEM *traveller = NULL;

		//Uses the huffman tree to decodify the compressed text
		for(; i < (size - 2);){
			traveller = huff_table->huffman_t;

			while(traveller->symbol == INVALID){
				if ((text[i] & mask_pack) >= UNITY)
					traveller = traveller->son_r;
				else
					traveller = traveller->son_l;
				mask_pack <<= UNITY;
				if (mask_pack == EMPTY){
					mask_pack = UNITY;
					i++;
				}
			}

			new_text = (char *) realloc(new_text, sizeof(char) * (new_size + 2));
			new_text[new_size++] = traveller->symbol;
		}
		//Now the last valid symbol, which is a special case
		for(; mask_pack < (UNITY << text[size - 1]);){
			traveller = huff_table->huffman_t;
			while(traveller->symbol == INVALID){
				if ((text[size - 2] & mask_pack) >= UNITY)
					traveller = traveller->son_r;
				else
					traveller = traveller->son_l;
				mask_pack <<= UNITY;
			}
			new_text = (char *) realloc(new_text, sizeof(char) * (new_size + 2));
			new_text[new_size++] = traveller->symbol;
		}
		//And the '\0'
		new_text[new_size++] = '\0';
		return new_text;
	}
	return NULL;
};

//Destroys all the huffman structure
void liberarTabelaHuffman(TABELA_HUFFMAN **huff_table){
	if (huff_table != NULL && *huff_table != NULL){
		if ((*huff_table)->symbols != NULL)
			free((*huff_table)->symbols);
		if ((*huff_table)->codes != NULL)
			free((*huff_table)->codes);
		if ((*huff_table)->huffman_t != NULL)
			huffman_destroy((*huff_table)->huffman_t);
		free(*huff_table);
		(*huff_table) = NULL;
	}
};