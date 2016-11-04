#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "sort.h"

typedef struct{
	short int *path_indexes;
	unsigned int path_size;
	float geodesic_dis;
} path_info;

struct chamber{
	struct chamber **paths;
	boolean end_chamber, marked;
	short int index, num_paths, paths_minus;
	float cd_x, cd_y;
};

struct maze{
	STACK *memory;
	path_info *current_path;
	struct chamber *start;
	unsigned short int num_dots, num_chambers, num_paths;
};

struct solutions{
	path_info **paths;
	unsigned int num_sol;
};

static CHAMBER *function_initChamber(short int index){
	CHAMBER *chamber = (CHAMBER *) malloc(sizeof(CHAMBER));
	if (chamber != NULL){
		chamber->index = index;
		
		scanf("%f%f", &chamber->cd_x, &chamber->cd_y);
		getchar();

		#ifdef DEBUG
			printf("D: initiallized a new chamber: %hd index, %f cd_x and %f cd_y.\n", 
				chamber->index, chamber->cd_x, chamber->cd_y);
		#endif

		chamber->paths = NULL;
		chamber->num_paths = EMPTY;
		chamber->paths_minus = EMPTY;
		chamber->end_chamber = FALSE;
		chamber->marked = FALSE;
	}
	return chamber;
};

static CHAMBER **get_entries(MAZE *maze){
	CHAMBER **list = NULL;
	if (maze != NULL){
		scanf("%hu", &maze->num_dots);
		getchar();
		list = (CHAMBER **) malloc(sizeof(CHAMBER *) * maze->num_dots);
		for(short int i = maze->num_dots - UNITY; i >= EMPTY; i--)
			list[i] = NULL;

		#ifdef DEBUG
			printf("D: There's %hu dots in this maze.\n", maze->num_dots);
		#endif

		if (list != NULL){
			short int i, aux_0, aux_1;

			//#0 - CREATES ALL DOTS
			for(i = EMPTY; i < maze->num_dots; i++){
				//Init a new chamber
				list[i] = function_initChamber(i + UNITY);

				if (list[i] != NULL)
					continue;

				#ifdef DEBUG
					printf("E: something went wrong. aborting chambers initiallization.\n");
				#endif
				//Something went wrong with memory allocation of a chamber, them destroy all chambers and return NULL.
				for(short int j = i; j >= EMPTY; j--){
					free(list[j]);
					free(list);
					return NULL;
				}
			}

			//#1 - INITIALIZE CHAMBERS
			scanf("%hu", &maze->num_chambers);
			getchar();

			#ifdef DEBUG
				printf("D: There's %hu chambers in this maze.\n", maze->num_chambers);
			#endif

			i = (maze->num_chambers);
			while (i > EMPTY){
				//Get information of which chambers are final chambers (with maze exit)
				scanf("%hd", &aux_0);
				--aux_0;
				if (aux_0 >= EMPTY){
					short int aux;
					scanf("%hd", &aux);
					getchar();
					list[aux_0]->end_chamber = (boolean) aux;

					#ifdef DEBUG
						printf("D: chamber #%hu 'maze end' set to %hd.\n", aux_0 + UNITY, aux);
					#endif

				}
				i--;
			}
			//#2 SET PATHS AND MAZE ITSELF
			scanf("%hu", &maze->num_paths);
			getchar();

			#ifdef DEBUG
				printf("D: There's %hu paths in this maze.\n", maze->num_paths);
			#endif

			i = (maze->num_paths);
			while(i > EMPTY){
				//Set all possible paths
				scanf("%hd%hd", &aux_0, &aux_1);
				getchar();
				//There's no need to setup blocked ways because they're always invalid and never could be part of a solution.
				if (aux_0 >= UNITY && aux_1 >= UNITY){
					//Since the index of chambers starts at 1, the list index must be aux_0 - 1 and 
					//aux_1 - 1, hence this little modification below.

					#ifdef DEBUG
						printf("D: new path set between [%hu-%hu] chambers.\n", aux_0, aux_1);
					#endif

					--aux_0;
					--aux_1;
					list[aux_0]->paths = (CHAMBER **) realloc(list[aux_0]->paths, sizeof(CHAMBER *) * (list[aux_0]->num_paths + UNITY));
					list[aux_0]->paths[list[aux_0]->num_paths++] = list[aux_1];
					list[aux_1]->paths = (CHAMBER **) realloc(list[aux_1]->paths, sizeof(CHAMBER *) * (list[aux_1]->num_paths + UNITY));
					list[aux_1]->paths[list[aux_1]->num_paths++] = list[aux_0];
				}
				i--;
			}

			#ifdef DEBUG
				for(int i = (maze->num_dots - UNITY); i >= EMPTY; i--)
					printf("D: There's %hu entries in chamber of index %hu.\n", list[i]->num_paths, i + UNITY);
			#endif

			//#3 Get the maze start 
			scanf("%hu", &aux_0);
			getchar();
			if (aux_0 >= UNITY)
				maze->start = list[--aux_0];

			#ifdef DEBUG
				printf("D: [%hu] is the index of the initial chamber of the maze (%p).\n", aux_0 + UNITY, maze->start);
			#endif
		}
	}
	//List will be kept to destroy maze structure at the end of the program
	return list;
};

MAZE *maze_init(CHAMBER ***chambers){
	MAZE *maze = (MAZE *) malloc(sizeof(MAZE));
	if (maze != NULL){
		maze->current_path = NULL;
		(*chambers) = get_entries(maze); 
		maze->memory = stack_create();

		if (*chambers == NULL || maze->memory == NULL)
			maze_destroy(&maze, chambers);
		#ifdef DEBUG
			if (*chambers != NULL && maze->memory != NULL)
				printf("D: successfully got entries and created maze cache memory structure.\n");
		#endif
	}
	return maze;
};

void maze_destroy(MAZE **maze, CHAMBER ***chambers){
	if (maze != NULL && *maze != NULL){
		if (chambers != NULL && *chambers != NULL){
			short int i = ((*maze)->num_dots - UNITY);
			while(i >= EMPTY){
				#ifdef DEBUG
					printf("D: destroying #%hd maze dot...\n", i);
				#endif
				if ((*chambers)[i] != NULL){
					if((*chambers)[i]->paths != NULL){
						free((*chambers)[i]->paths);
					}
					free((*chambers)[i]);
				}	
				i--;
			}
			free(*chambers);
			(*chambers) = NULL;

			#ifdef DEBUG
				printf("D: successfully completed chambers removal.\n");
			#endif
		}

		if((*maze)->current_path != NULL){
			if ((*maze)->current_path->path_indexes != NULL)
				free((*maze)->current_path->path_indexes);
			free((*maze)->current_path);
		}

		if ((*maze)->memory != NULL){
			#ifdef DEBUG
				printf("D: will clear maze structure's cache memory...\n");
			#endif
			stack_destroy((*maze)->memory);
		}

		free(*maze);
		(*maze) = NULL;

		#ifdef DEBUG
			printf("D: successfully destroyed maze structure.\n");
		#endif
	}
};

void maze_solutionsPrint(SOLUTIONS *const solutions){
	if (solutions != NULL && solutions->paths != NULL){
		for(unsigned int j = EMPTY; j < solutions->num_sol; j++){
			if (solutions->paths[j] != NULL){
				printf("%u", solutions->paths[j]->path_size);
				for(unsigned int i = EMPTY; i < solutions->paths[j]->path_size; i++)
					printf(" %hd", *(solutions->paths[j]->path_indexes + i));
				printf(" %d\n", (int) solutions->paths[j]->geodesic_dis);
			}
		}
	}
};

static void function_sortPaths(path_info **paths, unsigned int const size){
	for(int i = UNITY; i < size; i++)
		for(int j = EMPTY; j < size - i; j++)
			for(int k = EMPTY; k < paths[EMPTY]->path_size; k++)
				if (paths[j]->path_indexes[k] > paths[j + UNITY]->path_indexes[k])
					SWAP(paths[j], paths[j + UNITY]);
};

static void function_bucketSort(path_info **paths, unsigned int const size, short int const criteria){
	short int j, k;
	long int i = (size - UNITY);
	int times = EMPTY, greater = INVALID, aux_v;
	switch(criteria){
		case BUCKET_GEODESIC:
			while(i >= EMPTY){
				greater = MAX(greater, (int) paths[i]->geodesic_dis);
				i--;
			}
			break;
		case BUCKET_PATHSIZE:
			while(i >= EMPTY){
				greater = MAX(greater, (int) paths[i]->path_size);
				i--;
			}
			break;
		default:
			function_sortPaths(paths, size);
			return;
	}
	
	while(greater > EMPTY){
		greater /= BASE;
		times++;
	}

	path_info ***aux = (path_info ***) malloc(sizeof(path_info ***) * BASE);
	short int *counter = (short int *) malloc(sizeof(short int) * BASE); 
	for(j = (BASE - UNITY); j >= EMPTY; j--){
		aux[j] = NULL;
		counter[j] = EMPTY;
	}

	for(int level = UNITY; level <= times; level++){
		for(i = EMPTY; i < size; i++){
			aux_v = paths[i]->path_size;
			if (criteria == BUCKET_GEODESIC)
				aux_v = paths[i]->geodesic_dis;	
			
			for(j = level - UNITY; j > EMPTY; j--)
				aux_v /= BASE;
			aux_v %= BASE;
			aux[aux_v] = (path_info **) realloc(aux[aux_v], sizeof(path_info *) * (counter[aux_v] + UNITY));
			aux[aux_v][counter[aux_v]++] = paths[i];
		}

		if (times == level)
			for(i = EMPTY; i < BASE; i++)
				if(counter[i] > UNITY)
					function_bucketSort(aux[i], counter[i], criteria + UNITY);

		k = EMPTY;
		for(i = EMPTY; i < BASE; i++)
			for(j = EMPTY; j < counter[i]; j++)
				paths[k++] = aux[i][j];

		for(j = (BASE - UNITY); j >= EMPTY; j--){
			if (aux[j] != NULL)
				free(aux[j]);
			aux[j] = NULL;
			counter[j] = EMPTY;
		}
	}
	free(counter);
	free(aux);
};

void maze_solutionsSort(SOLUTIONS *solutions){
	if (solutions != NULL && solutions->paths != NULL)
		function_bucketSort(solutions->paths, solutions->num_sol, EMPTY);
};

void maze_solutionsDestroy(SOLUTIONS *solutions){
	if (solutions != NULL){
		if (solutions->paths != NULL){
			unsigned int i = (solutions->num_sol);
			while(i > EMPTY){
				if (solutions->paths[i - UNITY] != NULL){
					if (solutions->paths[i - UNITY]->path_indexes != NULL)
						free(solutions->paths[i - UNITY]->path_indexes);
					free(solutions->paths[i - UNITY]);
				}
				--i;
			}
			free(solutions->paths);
		}
		free(solutions);
	}
};

static path_info *function_pathInit(){
	path_info *path = (path_info *) malloc(sizeof(path_info));
	if (path != NULL){
		path->path_indexes = NULL;
		path->path_size = EMPTY;
		path->geodesic_dis = EMPTY;
	}
	return path;
};

static void function_addSolution(SOLUTIONS *solutions, path_info *const current_path){
	//This function creates a copy of a solution because the solutions must be printed sorted
	if (solutions != NULL && current_path != NULL){
		solutions->paths = (path_info **) realloc(solutions->paths, sizeof(path_info *) * (solutions->num_sol + UNITY));
		if (solutions->paths != NULL){
			solutions->paths[solutions->num_sol] = function_pathInit();
			path_info *aux = solutions->paths[solutions->num_sol];
			unsigned int aux_size = current_path->path_size;

			aux->geodesic_dis = current_path->geodesic_dis;
			aux->path_size = aux_size;
			aux->path_indexes = (short int *) malloc(sizeof(short int) * aux_size);
			if (aux->path_indexes != NULL)
				for(unsigned int i = EMPTY; i < aux_size; i++)
					aux->path_indexes[i] = current_path->path_indexes[i];
			solutions->num_sol++;
		}
	}
};

static SOLUTIONS *function_solutionsInit(){
	SOLUTIONS *solutions = (SOLUTIONS *) malloc(sizeof(SOLUTIONS));
	if (solutions != NULL){
		solutions->num_sol = EMPTY;
		solutions->paths = NULL;
	}
	return solutions;
};


/*
GO THROUGH MAZE WITH "BRUTE FORCE"
0. 	All adress of the chambers passed by must be stored in a vector 
1. 	All additional possible paths (num_paths - 1) in a chamber must be stored in the stack memory 
	and accessed after a dead-end or a successfully maze exit.
2.	If chamber->end_chamber == TRUE, print all the chamber indexes in the current vector. After this:
	- If the stack is empty, game ends.
	- Else, go back through the reverse vector and mark OFF the way until a chamber stacked is founded.
3. 	Game ends when stack is empty in a dead-end or a successfully exit
4.	To avoid cyclic infinite paths, all chambers passed by are marked.
5. 	If a dead-end is reached, do 2 without printing current vector.
*/

SOLUTIONS *maze_searchPaths(MAZE *maze, CHAMBER **chambers){
	SOLUTIONS *solutions = function_solutionsInit();

	#ifdef DEBUG
		printf("D: started search for solutions...\n");
	#endif

	if (solutions != NULL && maze != NULL && chambers != NULL){
		CHAMBER *traveller = maze->start;
		STACK *tracking = stack_create();
		if (traveller != NULL){
			maze->current_path = function_pathInit();
			if (maze->current_path != NULL){
				while(TRUE){
					//#0 - 	Add the current chamber to the current path
					maze->current_path->path_indexes = (short int *) realloc(maze->current_path->path_indexes,
						sizeof(short int) * (maze->current_path->path_size + UNITY));
					maze->current_path->path_indexes[maze->current_path->path_size++] = traveller->index;

					//#1 - mark current chamber, add a unity in path size and stack it due to a possible backtrack
					traveller->marked = TRUE;
					stack_push(tracking, traveller);

					#ifdef DEBUG
						printf("D: traveller index: %hu\n", traveller->index);
					#endif

					//#2 - 	Check if the current chamber has a maze exit. If true, print the current path and proceed.
					boolean FLAG_REC = FALSE;
					if (traveller->end_chamber){
						FLAG_REC = TRUE;
						function_addSolution(solutions, maze->current_path);
					}

					//#3 - 	Check if the current chamber has alternative ways. If true, stack current chamber to keep its adress.
					if ((traveller->num_paths - traveller->paths_minus) > 2){
						//Then we stack the current chamber to get back here in future
						CHAMBER *aux = traveller;
						stack_push(maze->memory, traveller);
						//Now, we must choice a valid way to proceed.
						traveller = aux->paths[aux->num_paths - UNITY];
						++(aux->paths_minus);
						while(traveller->marked && (aux->num_paths) > aux->paths_minus){
							SWAP(traveller->paths[traveller->num_paths - 1], traveller->paths[EMPTY]);
							for(int k = traveller->num_paths - 2; k > EMPTY; k--)
								SWAP(traveller->paths[k], traveller->paths[k + 1]);
							traveller = aux->paths[(aux->num_paths - UNITY)];
							++(aux->paths_minus);
						}
						//We founded a valid way, it's time to calculate the geodesic distance between the chambers:
						maze->current_path->geodesic_dis += EUCLIDEAN(traveller, stack_top(maze->memory));
						//And then go the selected way in the next loop.
					} else{
						//There's no alternative ways. In that situation, we must have two possibilities:
						if ((traveller->num_paths - traveller->paths_minus) == UNITY && traveller->paths[EMPTY]->marked){
							//There's no another possible way, i.e we reached a dead end. By now, there's two possibilities:
							if (!stack_empty(maze->memory) /*&& stack_top(maze->memory) != traveller*/){
								//a) Another way was left back, then backtrack and check that way.
								//Backtrack here.
								do{
									traveller->marked = FALSE;
									maze->current_path->path_indexes = (short int *) realloc(maze->current_path->path_indexes,
										sizeof(short int) * (maze->current_path->path_size--));
									CHAMBER *aux_2 = traveller;
									traveller = stack_pop(tracking);
									if (FLAG_REC && traveller->paths_minus > EMPTY){
										SWAP(traveller->paths[traveller->num_paths - 1], traveller->paths[EMPTY]);
										for(int k = traveller->num_paths - 2; k > EMPTY; k--)
											SWAP(traveller->paths[k], traveller->paths[k + 1]);
										traveller->paths_minus--;
									}
									maze->current_path->geodesic_dis -= EUCLIDEAN(traveller, aux_2);
								} while(!stack_empty(tracking) && traveller != stack_top(maze->memory));
								stack_pop(maze->memory);
							}else{
								//b) No way was left back, then operation is completed. Return.
								stack_destroy(tracking);
								return solutions;
							}
						}else{
							//There's only one possible way.
							CHAMBER *aux = traveller->paths[EMPTY];
							if (aux->marked){
								SWAP(traveller->paths[EMPTY], traveller->paths[UNITY]);
								aux = traveller->paths[EMPTY];
							}
							//First, check if it's not a cyclic way:
							if (aux->marked){
								//If TRUE, this situation is pretty much like a dead-end.
								if (!stack_empty(maze->memory)){
									//Even if the stack memory has a adress in it, does not necessarily mean 
									//that has another way on maze to check. Then to be sure:
									boolean FLAG = TRUE;
									while(FLAG){
										FLAG = FALSE;
										if (stack_top(maze->memory) != aux){
											//a) Another way was left back, then backtrack and check that way.
											//Backtrack here.
											do{
												traveller->marked = FALSE;
												maze->current_path->path_indexes = (short int *) realloc(maze->current_path->path_indexes,
													sizeof(short int) * (maze->current_path->path_size--));
												CHAMBER *aux_2 = traveller;

												traveller = stack_pop(tracking);
												if (FLAG_REC && traveller->paths_minus > EMPTY){
													SWAP(traveller->paths[traveller->num_paths - 1], traveller->paths[EMPTY]);
													for(int k = traveller->num_paths - 2; k > EMPTY; k--)
														SWAP(traveller->paths[k], traveller->paths[k + 1]);
													traveller->paths_minus--;
												}
												
												maze->current_path->geodesic_dis -= EUCLIDEAN(traveller, aux_2);
											}while(!stack_empty(tracking) && traveller != stack_top(maze->memory));
											stack_pop(maze->memory);
										} else {
											//Force pop of stack memory and double-check it:
											stack_pop(maze->memory);
											if (!stack_empty(maze->memory))
												FLAG = TRUE;
										}
									}//FLAG loops end here
								}else{
									//b) No way was left back, then operation is completed. Return.
									stack_destroy(tracking);
									return solutions;
								}
							}else{
								//If not, then calculate the geodesic distance and proceed.
								maze->current_path->geodesic_dis += EUCLIDEAN(traveller, aux);
								traveller = aux;
							}
						}
					}
				}
			}
		}
	}
	return solutions;
};