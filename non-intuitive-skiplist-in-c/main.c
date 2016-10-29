#include <stdlib.h>
#include <stdio.h>
#include "skiplist.h"
#include <time.h>

#define DEBUG

int	main(int argc, char *argv[]){
	SKIPLIST *skiplist = skiplist_createStruct();
	NODE *node = NULL;
	clock_t clk_str, clk_end;

	#ifdef DEBUG
		long double time_total;
		int counter = EMPTY;
	#endif

	if(skiplist != NULL){
		printf("successfully created skiplist structure...\n");
		COMMANDS *commands = function_createCommands();
		if (commands != NULL){
			printf("successfully created commands structure...\n");
			while(function_getValue(commands) != EXIT_PROGRAM){
				printf("Please insert a valid command: (type 'c' for list)\n");
				if(!function_cleanCommands(commands))
					printf("W: failed to clean command table.\n");
				if(!function_getCommands(commands))
					printf("W: failed to get commands.\n");
				switch(function_getValue(commands)){
					case CMD_COMMANDS:
						function_dumpCommands();
						break;
					case CMD_INSERT:
						if(skiplist_insert(skiplist, commands)){
							printf("successfully inserted new element.\n");
							#ifdef DEBUG
								printf("%d\n", counter++);
							#endif
						}
						else
							printf("E: failed to insert new element.\n");
						break;
					case CMD_SEQSEARCH:
						printf("W: sequential search can be slower than skipsearch.\n");
						clk_str = clock();
						node = skiplist_seqSearch(skiplist, commands);
						if (node != NULL)
							printf("found item! It's IP is %s.\n", function_getNodeIp(node));
						else 
							printf("E: can't find requested key.\n");
						clk_end = clock();
						#ifdef DEBUG
							time_total = (clk_end - clk_str)/(CLOCKS_PER_SEC * 1.0);
							printf("Total time spended: %.4Lf secs (%.4Lf minutes/ %.4Lf hours).\n", time_total, time_total/60, time_total/3600);
						#endif
						function_destroyArgs(commands);
						node = NULL;
						break;
					case CMD_REMOVE:
						if(skiplist_remove(skiplist, commands))
							printf("successfully removed element.\n");
						else
							printf("E: failed to remove element.\n");
						function_destroyArgs(commands);
						break;
					case CMD_SEARCH:
						printf("started searching...\n");
						clk_str = clock();
						node = skiplist_search(skiplist, commands);
						if (node != NULL)
							printf("found item! It's IP is %s.\n", function_getNodeIp(node));
						else 
							printf("E: can't find requested key.\n");
						function_destroyArgs(commands);
						node = NULL;
						clk_end = clock();
						#ifdef DEBUG
							time_total = (clk_end - clk_str)/(CLOCKS_PER_SEC * 1.0);
							printf("Total time spended: %.4Lf secs (%.4Lf minutes/ %.4Lf hours).\n", time_total, time_total/60, time_total/3600);
						#endif
						break;
					case CMD_MAPNODES: 
						skiplist_mapnodes(skiplist);
						break;
					case EXIT_PROGRAM:
						printf("program is now exiting...\n");
						break;
					default: 
						printf("E: command unavaliable.\n"); 
						break;
				}
			}
			if(function_destroyCommands(&commands))
				printf("succesfully deallocated command structure memory...\n");
			else
				printf("Can't destroy commands structure.\n");
		} else printf("E: can't create commands structure.\n");
		if(skiplist_destroy(&skiplist))
			printf("succesfully destroyed skiplist structure...\n");
		else
			printf("Can't destroy skiplist structure.\n");
	} else printf("E: can't create skiplist structure.\n");
	printf("Program endded.\n");
	return 0;
}