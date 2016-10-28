#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

#define DEBUG

struct node{
	unsigned char value;
	unsigned int freq;
	unsigned short int level;
	struct node *s_left, *s_right;
};

struct huffman_t{
	NODE *root;
};

typedef struct{
	unsigned short int *m_list, *m_levels, codeMask, codePackage, level;
	FILE *foutput; 
} codeInformation;

HUFFMAN_T *huffmanInit(){
	#ifdef DEBUG
		printf("D: Trying to init huffman tree...\n");
	#endif
	HUFFMAN_T *huffman_t = (HUFFMAN_T *) malloc(sizeof(HUFFMAN_T));
	if (huffman_t != NULL)
		huffman_t->root = NULL;
	return huffman_t;
};

static NODE **function_hashInit(){
	#ifdef DEBUG
		printf("D: Trying to init hashTable...\n");
	#endif
	//It's garanteed that the frequency of first node is always zero, for future for otimization.
	NODE **hashTable = (NODE **) malloc(sizeof(NODE *) * (ASCII_SIZE + UNITY));
	if (hashTable != NULL){
		short int i = ASCII_SIZE;
		while(i >= EMPTY){
			hashTable[i] = (NODE *) malloc(sizeof(NODE));
			if (hashTable[i] != NULL){
				hashTable[i]->freq = EMPTY;
				hashTable[i]->value = i;
				hashTable[i]->s_left = NULL;
				hashTable[i]->s_right = NULL;
				hashTable[i]->level = EMPTY;
				i--;
			} else {
				#ifdef DEBUG
					printf("D: Failed to init hashTable.\n");
				#endif
				short int j = (i + UNITY);
				while(j <= ASCII_SIZE)
					free(hashTable[j++]);
				free(hashTable);
				hashTable = NULL;
			}
		}
	}
	return hashTable;
}

static void function_quickSort(NODE **hashTable, short int start, short int end){
	short int i = start, j = end, pivot = hashTable[(start + end)/2]->freq;
	while(j >= i){
		while(j > start && pivot < hashTable[j]->freq)
			j--;
		while(i < end && pivot > hashTable[i]->freq)
			i++;
		if (j >= i){
			SWAP_ADRESS(hashTable[i], hashTable[j]);
			i++;
			j--;
		}
	}
	if (i < end)
		function_quickSort(hashTable, i, end);
	if (j > start)
		function_quickSort(hashTable, start, j);
};

static void function_hashDestroy(NODE **hashTable, short int i){
	#ifdef DEBUG
		printf("D: preparing to destroy hashTable...\n");
	#endif
	if (hashTable != NULL && *hashTable != NULL){
		while (i >= EMPTY){
			free(hashTable[i]);
			i--;
		}
		free(hashTable);
	}
}

static boolean function_preCompress(HUFFMAN_T *huffman_t, FILE *finput){
	#ifdef DEBUG
		printf("D: now in function: _preCompress\n");
	#endif
	if (huffman_t != NULL && finput != NULL){
		NODE **hashTable = function_hashInit();
		if (hashTable != NULL){

			#ifdef DEBUG
				printf("D: hashTable created succesfully. Now going to count elements in input file...\n");
			#endif

			//Hashing elements on the input file
			char c;
			while(!feof(finput)){
				c = fgetc(finput);
				if (c != EOF)
					hashTable[(int) c]->freq++; 
			}
			
			//Sort all the elements by frequency in crescent order
			#ifdef DEBUG
				printf("D: preparing to sort elements with quickSort...\n");
			#endif
			
			function_quickSort(hashTable, EMPTY, ASCII_SIZE);

			#ifdef DEBUG
				printf("D: Sort completed. Result (ASCII value, freq):\n");
				short int K = ASCII_SIZE;
				while(K >= EMPTY){
					printf("[%d](%u,%u)", K, hashTable[K]->value, hashTable[K]->freq);
					K--;
				}
				printf("\nD: Now preparing huffman tree...\n");
			#endif

			//Putting all non-null frequency sorted elements on the tree
			short int i = (ASCII_SIZE), j = INVALID;
			NODE *new_node = NULL;
			if (hashTable[i]->freq > EMPTY){
				//It's garanteed now that at least one element exists in input file.
				while(hashTable[i - UNITY] != NULL){
					i--;
					//This creates a loop to creates huffman tree.
					if (hashTable[i - UNITY] == NULL || hashTable[i - UNITY]->freq == EMPTY){
						//i++;
						if (j == INVALID)
							j = (i - UNITY);
						if (ASCII_SIZE != i){
							//Its garanteed that exists at least two different nodes to merge
							new_node = (NODE *) malloc(sizeof(NODE));
							//right son must be the node with larger frequency 
							new_node->s_right = hashTable[i + UNITY];//larger frequency
							new_node->s_left = hashTable[i];//smaller frequency
							new_node->value = ASCII_SIZE;//This is a invalid value in ASCII (128)

							#ifdef DEBUG
								//This debug function shows the progress of the tree construction
								printf("D: s_left f: %u, s_right f: %u, sum: %u\n", new_node->s_left->freq, 
									new_node->s_right->freq, new_node->s_left->freq + new_node->s_right->freq);
							#endif
							//The frequency of the new node is the sum of both less frequent nodes
							new_node->freq = (new_node->s_left->freq + new_node->s_right->freq);
							hashTable[i + UNITY] = new_node;
							hashTable[i] = NULL;
							//sorting the Hashtable once again...
							function_quickSort(hashTable, i + UNITY, ASCII_SIZE);
						}
						i = ASCII_SIZE;
					}
				}

				huffman_t->root = hashTable[ASCII_SIZE];
			}
			
			//Freeing memory from hash table
			function_hashDestroy(hashTable, j);

			#ifdef DEBUG
				printf("D: Process of hashTable destruction is completed. Returning to main...\n");
			#endif

			return TRUE;
		}
	}
	return FALSE;
};

#ifdef DEBUG
	static void function_debug_PrintTree(NODE *root){
		if (root != NULL){
			if (root->s_left != NULL){
				printf(", {(%u, %u)", root->s_left->value, root->s_left->freq);
				function_debug_PrintTree(root->s_left);
				printf("}");
			}
			if (root->s_right != NULL){
				printf(", {(%u, %u)", root->s_right->value, root->s_right->freq);
				function_debug_PrintTree(root->s_right);
				printf("}");
			}
		}
	};
#endif

static void function_writeList(NODE *root, codeInformation *info){
	#ifdef DEBUG
		printf("D: codePackage: %u, codeMask: %u, level: %u\n", info->codePackage, info->codeMask, info->level);
	#endif
	if (root != NULL){
		if (root->value != ASCII_SIZE){
			//Found a leaf node
			info->m_list[root->value] = info->codePackage;
			info->m_levels[root->value] = info->level;
			root->level = info->level;

			//Printing the compress code on output file
			fwrite(&root->value, sizeof(unsigned char), UNITY, info->foutput);
			fwrite(&root->level, sizeof(unsigned short int), UNITY, info->foutput);
			fwrite(&info->codePackage, sizeof(unsigned short int), UNITY, info->foutput);

			#ifdef DEBUG
				printf("D: modifying a legible output file 'HUFF_DEBUG.out' for debug purpose...\n");
				FILE *LEGIBLE_OUTPUT = fopen("HUFF_DEBUG.out", "a+");
				if (LEGIBLE_OUTPUT != NULL){
					fprintf(LEGIBLE_OUTPUT, "%c%u|", root->value, root->level);
					printf("D: created a new compress for symbol (%c): ", root->value);
					for(unsigned short int test_mask = MAX_SYMBOL_SHIFT; test_mask > (MAX_SYMBOL_SHIFT >> info->level); test_mask >>= UNITY){
						fprintf(LEGIBLE_OUTPUT, "%u", ((info->m_list[root->value] & test_mask) >= UNITY));
						printf("%u", ((info->m_list[root->value] & test_mask) >= UNITY));
					}
					printf("\n");
					fprintf(LEGIBLE_OUTPUT, "\n");
					fclose(LEGIBLE_OUTPUT);
				}
			#endif
		}else{
			info->level++;
			if (root->s_left != NULL){
				info->codeMask >>= UNITY;
				function_writeList(root->s_left, info);
				info->codeMask <<= UNITY;
			}
			if (root->s_right != NULL){
				info->codePackage |= info->codeMask;
				info->codeMask >>= UNITY;
				function_writeList(root->s_right, info);
				info->codeMask <<= UNITY;
				info->codePackage &= ~(info->codeMask);
			}
			info->level--;
		}
	}
};

void huffmanCompress(HUFFMAN_T *huffman_t, FILE *finput, FILE *foutput){
	if (huffman_t != NULL && finput != NULL && foutput != NULL){
		//This functions constructs the huffman tree for compactation
		function_preCompress(huffman_t, finput);
		#ifdef DEBUG
			printf("D: completed huffman tree construction. Result:\n{");
			if (huffman_t->root != NULL){
				printf("(%u, %u)", huffman_t->root->value, huffman_t->root->freq);
				function_debug_PrintTree(huffman_t->root);
			}
			printf("}\nD: now creating mask list with compress symbols...\n");
		#endif

		//This list will kept the code of each mask to compress the file
		codeInformation *info = (codeInformation *) malloc(sizeof(codeInformation));
		if (info != NULL){
			info->m_list = (unsigned short int *) malloc(sizeof(unsigned short int) * (ASCII_SIZE + UNITY));
			info->m_levels = (unsigned short int *) malloc(sizeof(unsigned short int) * (ASCII_SIZE + UNITY));
			info->codeMask = MAX_SYMBOL_SHIFT;
			info->codePackage = EMPTY;
			info->level = EMPTY;
			info->foutput = foutput;
			if (info->m_list != NULL && info->m_levels != NULL){
				function_writeList(huffman_t->root, info);
				#ifdef DEBUG
					printf("D: mask list successfully created.\nD: Verifying real output file:\n");
					unsigned int size = ftell(info->foutput);
					printf("D: There's %u characters on output...\n", size);
					fseek(info->foutput, EMPTY, SEEK_SET);
					unsigned char test_a;
					unsigned short int test_b, test_c;
					while(size > EMPTY){
						fread(&test_a, sizeof(unsigned char), UNITY, info->foutput);
						size -= sizeof(unsigned char);
						if (test_a != EOF){
							fread(&test_b, sizeof(unsigned short int), UNITY, info->foutput);
							fread(&test_c, sizeof(unsigned short int), UNITY, info->foutput);
							size -= 2*sizeof(unsigned short int);

							printf("(%c) (%u) (%u)[", test_a, test_b, test_c);
							for(unsigned short int test_mask = MAX_SYMBOL_SHIFT; test_mask > (MAX_SYMBOL_SHIFT >> test_b); test_mask >>= UNITY)
								printf("%u", ((test_c & test_mask) >= UNITY));
							printf("]\n");
						} else break;
					}
					printf("D: now compressing file...\n");
				#endif

				//File compression
				unsigned char package = EMPTY, mask_package = MAX_MASK_SHIFT;
				unsigned short int mask_in;
				char c_in;

				fseek(finput, EMPTY, SEEK_END);
				long long int f_size = ftell(finput);
				fseek(finput, EMPTY, SEEK_SET);
				
				#ifdef DEBUG
					printf("D: the input file has %lld characters...\n", f_size);
				#endif

				while(f_size >= EMPTY){
					c_in = fgetc(finput);
					if (c_in != EOF){
						for(mask_in = MAX_SYMBOL_SHIFT; mask_in > (MAX_SYMBOL_SHIFT >> info->m_levels[(unsigned) c_in]); mask_in >>= UNITY){
							if(mask_package == EMPTY){
								fwrite(&package, sizeof(unsigned char), UNITY, foutput);
								mask_package = MAX_MASK_SHIFT;
								package = EMPTY;
							}

							if ((info->m_list[(unsigned) c_in] & mask_in) >= UNITY)
								package |= mask_package;
							mask_package >>= UNITY;
						}
					}
					f_size--;
				}

				#ifdef DEBUG
					long long int TEST_SIZE_OUT = ftell(foutput), TEST_SIZE_IN = ftell(finput);
					printf("D: output file has %lld characters (compression of %.4lf%%).\n", 
						TEST_SIZE_OUT, (UNITY - TEST_SIZE_OUT/(TEST_SIZE_IN * 1.0)) * 100.0);
				#endif

				if (mask_package >= UNITY)
					fwrite(&package, sizeof(unsigned char), UNITY, foutput);
				#ifdef DEBUG
					printf("D: compression terminated. Freeing memories and returning to main...\n");
				#endif
				free(info->m_levels);
				free(info->m_list);
			}
			free(info);
			info = NULL;
		}
	}
};

static boolean function_preDecompress(HUFFMAN_T *huffman_t, FILE *finput){
	if (huffman_t != NULL && finput != NULL){
		
		return TRUE;
	}
	return FALSE;
};

void huffmanDecompress(HUFFMAN_T *huffman_t, FILE *finput, FILE *foutput){
	if (huffman_t != NULL && finput != NULL && foutput != NULL){
		function_preDecompress(huffman_t, finput);
	}
};

static void function_destroyTree(NODE *root){
	if (root != NULL){
		function_destroyTree(root->s_left);
		function_destroyTree(root->s_right);
		free(root);
	}
};

boolean huffmanDestroy(HUFFMAN_T **huffman_t){

	#ifdef DEBUG
		printf("D: Preparing to destroy huffman tree...\n");
	#endif

	if (huffman_t != NULL && *huffman_t != NULL){
		function_destroyTree((*huffman_t)->root);
		free(*huffman_t);
		(*huffman_t) = NULL;
		return TRUE;
	}
	return FALSE;
};