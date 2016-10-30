#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

//#define DEBUG

struct node{
	byte value, level;
	unsigned int freq;
	struct node *s_left, *s_right;
};

struct huffman_t{
	NODE *root;
};

typedef union{
	l_byte code; 
	byte memory_byte[sizeof(l_byte)];
} type_mask;

typedef struct{
	type_mask codePackage, *m_list;
	byte codeMask, *m_levels, level;
	FILE *foutput; 
} codeInformation;

typedef struct{
	byte *symbol, *level;
	type_mask *code;
} decodifyTable;

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

static void function_quickSort(NODE **hashTable, short int const start, short int const end){
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

static boolean function_preCompress(HUFFMAN_T *huffman_t, FILE *const finput){
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
				#ifdef DEBUG
				printf("TEST: (symbol: %c/unsigned: %u/signed: %d)\n", c, c, c);
				#endif
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
				printf("D: Process of hashTable destruction is completed. Returning to compression function...\n");
			#endif

			return TRUE;
		}
	}
	return FALSE;
};

#ifdef DEBUG
	static void function_debug_PrintTree(NODE *root, byte level){
		if (root != NULL){
			if (root->s_left != NULL){
				printf(", [<-, %u]{(%u, %u)", level, root->s_left->value, root->s_left->freq);
				function_debug_PrintTree(root->s_left, level + UNITY);
				printf("}");
			}
			if (root->s_right != NULL){
				printf(", [->, %u]{(%u, %u)", level, root->s_right->value, root->s_right->freq);
				function_debug_PrintTree(root->s_right, level + UNITY);
				printf("}");
			}
		}
	};
#endif

static void function_writeList(NODE *root, codeInformation *info){
	#ifdef DEBUG
		printf("D: codePackage: %u, codeMask: %u, level: %u\n", info->codePackage.code, info->codeMask, info->level);
	#endif

	if (info->codeMask == EMPTY)
		info->codeMask = UNITY;
	boolean FLAG = FALSE;
	if (info->level >= BITS_IN_BYTE)
		FLAG = TRUE;

	if (root != NULL){
		if (root->value != ASCII_SIZE){
			//Found a leaf node
			info->m_list[root->value].code = info->codePackage.code;
			info->m_levels[root->value] = info->level;
			root->level = info->level;

			//Printing the compress code on output file
			fwrite(&root->value, sizeof(byte), UNITY, info->foutput);
			fwrite(&root->level, sizeof(byte), UNITY, info->foutput);
			fwrite(&info->codePackage.code, sizeof(l_byte), UNITY, info->foutput);
		}else{
			info->level++;
			if (root->s_left != NULL){
				//info->codeMask <<= UNITY;
				info->codeMask = ((info->codeMask << UNITY) == EMPTY) ? UNITY : (info->codeMask << UNITY);
				function_writeList(root->s_left, info);
				info->codeMask = ((info->codeMask >> UNITY) == EMPTY) ? MAX_MASK_SHIFT : (info->codeMask >> UNITY);
				//info->codeMask >>= UNITY;
			}
			if (root->s_right != NULL){
				info->codePackage.memory_byte[FLAG] |= info->codeMask;
				//info->codeMask <<= UNITY;
				info->codeMask = ((info->codeMask << UNITY) == EMPTY) ? UNITY : (info->codeMask << UNITY);
				function_writeList(root->s_right, info);
				info->codeMask = ((info->codeMask >> UNITY) == EMPTY) ? MAX_MASK_SHIFT : (info->codeMask >> UNITY);
				//info->codeMask >>= UNITY;
				info->codePackage.memory_byte[FLAG] &= ~(info->codeMask);
			}
			info->level--;
		}
	}
};

void huffmanCompress(HUFFMAN_T *huffman_t, FILE *const finput, FILE *foutput){
	if (huffman_t != NULL && finput != NULL && foutput != NULL){
		//This functions constructs the huffman tree for compactation
		function_preCompress(huffman_t, finput);
		#ifdef DEBUG
			printf("D: completed huffman tree construction. Result:\n{");
			if (huffman_t->root != NULL){
				printf("(%u, %u)", huffman_t->root->value, huffman_t->root->freq);
				function_debug_PrintTree(huffman_t->root, UNITY);
			}
			printf("}\nD: now creating mask list with compress symbols...\n");
		#endif

		//This list will kept the code of each mask to compress the file
		codeInformation *info = (codeInformation *) malloc(sizeof(codeInformation));
		if (info != NULL){
			info->m_list = (type_mask *) malloc(sizeof(type_mask) * (ASCII_SIZE + UNITY));
			info->m_levels = (byte *) malloc(sizeof(byte) * (ASCII_SIZE + UNITY));
			info->codeMask = UNITY;
			info->codePackage.code = EMPTY;
			info->level = EMPTY;
			info->foutput = foutput;
			if (info->m_list != NULL && info->m_levels != NULL){
				//This function travels through all the tree and create a cache table to speed up the compact process
				function_writeList(huffman_t->root, info);
				//Creating a terminator character to the table
				byte c_terminator = MAX_BYTE_VAL;
				fwrite(&c_terminator, sizeof(byte), UNITY, foutput);

				#ifdef DEBUG
					printf("D: mask list successfully created.\nD: Verifying real output file:\n");
					unsigned int size = ftell(info->foutput) - sizeof(byte);
					printf("D: There's %u characters on output...\n", size);
					fseek(info->foutput, EMPTY, SEEK_SET);
					byte test_a, test_b;
					type_mask test_c;
					while(size > EMPTY){
						fread(&test_a, sizeof(byte), UNITY, info->foutput);
						size -= sizeof(byte);
						if (test_a != EOF){
							fread(&test_b, sizeof(byte), UNITY, info->foutput);
							fread(&test_c.code, sizeof(l_byte), UNITY, info->foutput);
							size -= sizeof(byte)+sizeof(l_byte);

							printf("(%c) (%u) (%u)[", test_a, test_b, test_c.code);
							for(int j = 0; j < sizeof(l_byte); j++){
								for(byte test_mask = UNITY; test_mask != EMPTY; test_mask <<= UNITY)
									printf("%u", ((test_c.memory_byte[j] & test_mask) >= UNITY));
								printf("|");
							}
							printf("]\n");
						} else break;
					}
					fseek(info->foutput, EMPTY, SEEK_END);
					printf("D: now compressing file...\n");
				#endif

				fseek(finput, EMPTY, SEEK_END);
				long long int f_size = ftell(finput);
				fseek(finput, EMPTY, SEEK_SET);
				
				#ifdef DEBUG
					printf("D: the input file has %lld characters...\n", f_size);
				#endif

				//File compression
				type_mask package;
				package.code = EMPTY;
				byte package_mask = UNITY, actual_mask;
				boolean FLAG0 = FALSE, FLAG1;
				char c;
				while(f_size > EMPTY){
					c = fgetc(finput);
					if (c != EOF){
						FLAG1 = FALSE;
						actual_mask = UNITY;
						for(l_byte aux = UNITY; aux < (UNITY << info->m_levels[(unsigned) c]); aux <<= UNITY){
							if (package_mask == EMPTY){
								if (FLAG0 == TRUE){

									#ifdef DEBUG
										printf("D: will write the following compressed symbol (code was like in file):");
										for(int j = 0; j < sizeof(l_byte); j++){
											for(byte test_mask = MAX_MASK_SHIFT; test_mask != EMPTY; test_mask >>= UNITY)
												printf("%u", ((package.memory_byte[j] & test_mask) >= UNITY));
											printf("|");
											}
										printf("\n");
									#endif

									fwrite(&package.code, sizeof(l_byte), UNITY, foutput);
									package.code = EMPTY;
								}
								package_mask = UNITY;
								FLAG0 = !FLAG0;
							}
							if (actual_mask == EMPTY){
								FLAG1 = TRUE;
								actual_mask = UNITY;
							}
							if((info->m_list[(unsigned) c].memory_byte[FLAG1] & actual_mask) >= UNITY)
								package.memory_byte[FLAG0] |= package_mask;
							package_mask <<= UNITY;
							actual_mask <<= UNITY;
						}
					}
					f_size--;
				}

				//Checking for incomplete code packs
				for(int k = EMPTY; k < sizeof(byte); k++)
					if (package.memory_byte[k] >= UNITY)
						fwrite(&package.memory_byte[k], sizeof(byte), UNITY, foutput);

				#ifdef DEBUG
					long long int TEST_SIZE_OUT = ftell(foutput), TEST_SIZE_IN = ftell(finput);
					printf("D: output file has %lld characters (compression of %.4lf%%).\n", 
						TEST_SIZE_OUT, (UNITY - TEST_SIZE_OUT/(TEST_SIZE_IN * 1.0)) * 100.0);
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

static NODE *function_nodeInit(){
	NODE *node = (NODE *) malloc(sizeof(NODE));
	if (node != NULL){
		node->value = ASCII_SIZE;
		node->s_right = NULL;
		node->s_left = NULL;
	}
	return node;
}

static void function_buildUp(HUFFMAN_T *huffman_t, decodifyTable* d_table, l_byte index){
	if (huffman_t->root == NULL)
		huffman_t->root = function_nodeInit();

	byte codeMask = UNITY;
	NODE *root = huffman_t->root;
	boolean FLAG = FALSE;
	
	while(d_table->level[index] > EMPTY){
		if ((d_table->code[index].memory_byte[FLAG] & codeMask) >= UNITY){
			if (root->s_right == NULL)
				root->s_right = function_nodeInit();
			root = root->s_right;
		}else{
			if (root->s_left == NULL)
				root->s_left = function_nodeInit();
			root = root->s_left;
		}
		codeMask <<= UNITY;

		if (codeMask == EMPTY){
			codeMask = UNITY;
			FLAG = TRUE;
		}
		d_table->level[index]--;
	}
	//Write the symbol of leaf node value
	root->value = d_table->symbol[index];
};

static HUFFMAN_T *function_preDecompress(FILE *const finput){
	if (finput != NULL){
		HUFFMAN_T *huffman_t = (HUFFMAN_T *) malloc(sizeof(HUFFMAN_T));
		if (huffman_t != NULL){
			decodifyTable *d_table = (decodifyTable *) malloc(sizeof(decodifyTable));
			if (d_table != NULL){
				d_table->symbol = (byte *) malloc(sizeof(byte) * (ASCII_SIZE));
				if (d_table->symbol != NULL){
					d_table->level = (byte *) malloc(sizeof(byte) * (ASCII_SIZE));
					if (d_table->level != NULL){
						d_table->code = (type_mask *) malloc(sizeof(type_mask) * (ASCII_SIZE));
						if (d_table->code != NULL) {
							byte c = EMPTY;
							#ifdef DEBUG
								printf("D: cleaning decodify table to show up...\n");
								short int i = (ASCII_SIZE - UNITY);
								while(i >= EMPTY){
									d_table->symbol[i] = EMPTY;
									d_table->code[i].code = EMPTY;
									d_table->level[i] = EMPTY;
									i--;
								}
								printf("D: now gathering information:\n");
							#endif

							//Gathering information from input file
							while(c != MAX_BYTE_VAL && !feof(finput)){
								fread(&c, sizeof(byte), UNITY, finput);
								#ifdef DEBUG
									printf("(%c/%u)", c, c);
								#endif
								if (c != MAX_BYTE_VAL){
									unsigned int aux_v = (unsigned int) c;
									d_table->symbol[aux_v] = c;
									fread(&d_table->level[aux_v], sizeof(byte), UNITY, finput);
									fread(&d_table->code[aux_v].code, sizeof(l_byte), UNITY, finput);
									#ifdef DEBUG
									printf("(%u)", d_table->level[aux_v]);
										printf("(%u)\n", d_table->code[aux_v].code);
									#endif
								}
							}
							
							#ifdef DEBUG
								if (d_table != NULL){
									printf("D: the decodify table is like: (symbol, level, code)\n");
									short int i = (ASCII_SIZE - UNITY);
									while(i >= EMPTY){
										printf("(%c, %u, ", d_table->symbol[i], d_table->level[i]);
										for(int j = 0; j < sizeof(l_byte); j++){
											for(byte test_mask = UNITY; test_mask != EMPTY; test_mask <<= UNITY)
												printf("%u", ((d_table->code[i].memory_byte[j] & test_mask) >= UNITY));
											printf("|");
										}
										printf(")\n");
										i--;
									}
									printf("D: now going to build up huffman tree...\n");
								}
							#endif

							//Now it's time to build up the tree using the decodify table.
							char aux_counter = (ASCII_SIZE - UNITY);
							while(aux_counter >= EMPTY){
								if (d_table->level[(unsigned) aux_counter] > EMPTY)
									function_buildUp(huffman_t, d_table, (unsigned) aux_counter);
								aux_counter--;
							}

							#ifdef DEBUG
								printf("D: huffman tree builded. Destroying decodify table and returning to decompress function...\n");
							#endif

							//freeing memory form d_table
							free(d_table->level);
							free(d_table->symbol);
							free(d_table->code);
							free(d_table);
							//Returning to decompress function
							return huffman_t;
						}
						free(d_table->level);
					}
					free(d_table->symbol);
				}
				free(d_table);
			}
			free(huffman_t);
		}
	}
	return NULL;
};

static unsigned char function_generateValue(HUFFMAN_T *huffman_t, FILE *const finput, 
	byte *input_mask, long long int *f_size, type_mask *input_code, boolean *FLAG){
	NODE *traveller = huffman_t->root;

	#ifdef DEBUG
		printf("D: the current symbol was:");
		for(int j = 0; j < sizeof(l_byte); j++){
			for(byte test_mask = UNITY; test_mask != EMPTY; test_mask <<= UNITY)
				printf("%u", (((*input_code).memory_byte[j] & test_mask) >= UNITY));
			printf("|");
			}
		printf("\n");
	#endif

	if (traveller != NULL){
		while(traveller != NULL && traveller->value == ASCII_SIZE){
			if (*input_mask == EMPTY){
				if (*FLAG == TRUE){
					fread(&input_code->code, sizeof(l_byte), UNITY, finput);
					*f_size -= sizeof(l_byte);
				}
				*input_mask = UNITY;
				*FLAG = !(*FLAG);
			}
			#ifdef DEBUG
				printf("[%u]", input_code->code);
				printf("(%u)", ((*input_code).memory_byte[*FLAG] & (*input_mask)) >= UNITY);
			#endif
			if (((*input_code).memory_byte[*FLAG] & (*input_mask)) >= UNITY)
				traveller = traveller->s_right;
			else
				traveller = traveller->s_left;
			(*input_mask) <<= UNITY;
		}
		if (traveller != NULL)
			return traveller->value;
	}
	#ifdef DEBUG
		printf("\n");
	#endif
	return ASCII_SIZE;
}

void huffmanDecompress(HUFFMAN_T *huffman_t, FILE *const finput, FILE *foutput){
	if (finput != NULL && foutput != NULL){
		//First, load the decodify table at the start of compressed file and transforms it into a huffman tree
		huffman_t = function_preDecompress(finput);

		if (huffman_t != NULL){
			#ifdef DEBUG
				printf("D: completed huffman tree construction. Result:\n{");
				if (huffman_t->root != NULL){
					printf("(%u, %u)", huffman_t->root->value, huffman_t->root->freq);
					function_debug_PrintTree(huffman_t->root, UNITY);
				}
				printf("}\nD: now generating output decompressed file...\n");
			#endif

			//Generating output file
			long long int aux = ftell(finput);
			fseek(finput, EMPTY, SEEK_END);
			long long int f_size = ftell(finput) - aux;
			fseek(finput, aux, SEEK_SET);

			#ifdef DEBUG
				printf("TEST: (%lld)\n", aux);
				printf("D: input file has %lld symbols in it.\n", f_size);
			#endif

			type_mask input_code;
			input_code.code = EMPTY;
			byte input_mask = UNITY, c_out = EMPTY;
			boolean FLAG = FALSE;

			fread(&input_code.code, sizeof(l_byte), UNITY, finput);
			f_size -= sizeof(l_byte);

			while(f_size >= EMPTY){
				c_out = function_generateValue(huffman_t, finput, &input_mask, &f_size, &input_code, &FLAG);
				if (c_out != ASCII_SIZE){
					#ifdef DEBUG
						printf("\nD: found a symbol: (%c/%u)\n", c_out, c_out);
					#endif
					fprintf(foutput, "%c", c_out);
				}
			}

			#ifdef DEBUG
				long long int SIZE_OUT = ftell(foutput);
				printf("D: proccess terminated. Output file has %lld characters.\nD: returning to main...\n", SIZE_OUT);
			#endif
		}
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