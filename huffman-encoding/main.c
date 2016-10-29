#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "huffman.h"

int main(int argc, char const *argv[]){
	if (argc == ARG_NUM){
		//check if destiny file already exists
		if (access(argv[ARG_FOUTPUT], F_OK) == INVALID){
			//Opening input file
			FILE *const finput = fopen(argv[ARG_FINPUT], "rb");
			if (finput != NULL){
				//Initiation of huffman tree
				HUFFMAN_T *huffman_t = huffmanInit(finput);
				if (huffman_t != NULL){
					//Creating output file
					#ifdef DEBUG
						FILE *foutput = fopen(argv[ARG_FOUTPUT], "wb+");
					#endif

					#ifndef DEBUG
						FILE *foutput = fopen(argv[ARG_FOUTPUT], "wb+");
					#endif
						
					if (foutput != NULL){
						//Checking for command and opening file to operate
						if (atoi(argv[ARG_OPERATION]) == DO_COMPRESS)
							huffmanCompress(huffman_t, finput, foutput);
						else if (atoi(argv[ARG_OPERATION]) == DO_DECOMPRESS)
							huffmanDecompress(huffman_t, finput, foutput);
						else printf("E: '%s' is a invalid operation.\n", argv[ARG_OPERATION]);
						//Closing output file
						fclose(foutput);
					} else printf("E: unable to create output file.\n");
					//Freeing memory usage
					huffmanDestroy(&huffman_t);
				} else printf("E: failed to create huffman tree.\n");
				//Closing input file
				fclose(finput);
				//exiting
				if(huffman_t == NULL)
					return SUCCESS;
			} else printf("E: unable to open input file.\n");
		} else printf("E: '%s' already exists. Give me another name or path, or remove it first.\n", argv[ARG_FOUTPUT]);
		return FAILURE;
	}
	//Basic instructions and exiting
	printf("usage: %s input output <%u to compress/%u to decompress>\n", 
		argv[ARG_PROGNAME], DO_COMPRESS, DO_DECOMPRESS);
	return FAILURE;
}