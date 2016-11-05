#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "sort.h"

/*
	NOTA: ESTE TRABALHO NÃO PRECISA SER CORRIGIDO POIS >NÃO< SOU DA TURMA.
*/

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
			printf("D: initiallized a new chamber: %hd index, %.3f cd_x and %.3f cd_y.\n", 
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

static CHAMBER *function_initMasterChamber(CHAMBER *start){
	CHAMBER *chamber = (CHAMBER *) malloc(sizeof(CHAMBER));
	if (chamber != NULL){
		chamber->index = INVALID;

		#ifdef DEBUG
			printf("D: initiallized a master chamber: %hd index, ? cd_x and ? cd_y.\n", 
				chamber->index);
		#endif

		chamber->paths = (CHAMBER **) malloc(sizeof(CHAMBER *));
		chamber->paths[EMPTY] = start;
		chamber->num_paths = UNITY;
		chamber->paths_minus = UNITY;
		chamber->end_chamber = FALSE;
		chamber->marked = TRUE;
	}
	return chamber;
};

static CHAMBER **get_entries(MAZE *maze){
	CHAMBER **list = NULL;
	if (maze != NULL){
		scanf("%hu", &maze->num_dots);
		getchar();
		list = (CHAMBER **) malloc(sizeof(CHAMBER *) * (UNITY + maze->num_dots));
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
			if (aux_0 >= UNITY){
				//maze->start = list[--aux_0];
				//Initializing master chamber (index -1)
				--aux_0;
				list[maze->num_dots] = function_initMasterChamber(list[aux_0]);
				list[aux_0]->paths = (CHAMBER **) realloc(list[aux_0]->paths, sizeof(CHAMBER *) * (list[aux_0]->num_paths + UNITY));
				list[aux_0]->paths[list[aux_0]->num_paths++] = list[maze->num_dots];
				maze->start = list[maze->num_dots];

				list[maze->num_dots]->cd_x = list[aux_0]->cd_x;
				list[maze->num_dots]->cd_y = list[aux_0]->cd_y;
			}

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

//Destroy the maze structure.
void maze_destroy(MAZE **maze, CHAMBER ***chambers){
	if (maze != NULL && *maze != NULL){
		if (chambers != NULL && *chambers != NULL){
			short int i = ((*maze)->num_dots);
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

//Print the solution registers.
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

//Auxiliary function of sort, in case of indexes are taken part of the sort criterium.
static void function_sortPaths(path_info **paths, unsigned int const size){
	for(int i = UNITY; i < size; i++)
		for(int j = EMPTY; j < (int) size - i; j++)
			for(int k = EMPTY; k < paths[EMPTY]->path_size && paths[j]->path_indexes[k] < paths[j + UNITY]->path_indexes[k]; k++)
				if (paths[j]->path_indexes[k] > paths[j + UNITY]->path_indexes[k]){
					SWAP(paths[j], paths[j + UNITY]);
					break;
				}
			
};

//Modified bucketSort to sort the solutions.
//This sort uses the idea of RadixSort, but each call its criteria changes.
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

//Call function of sort
void maze_solutionsSort(SOLUTIONS *solutions){
	if (solutions != NULL && solutions->paths != NULL)
		function_bucketSort(solutions->paths, solutions->num_sol, EMPTY);
};

//This function clean up the solution registers.
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

//This function init the 'current solution' tracker.
static path_info *function_pathInit(){
	path_info *path = (path_info *) malloc(sizeof(path_info));
	if (path != NULL){
		path->path_indexes = NULL;
		path->path_size = EMPTY;
		path->geodesic_dis = EMPTY;
	}
	return path;
};

//This function make sure that the same solution is not registered multiple times.
static boolean function_validateSolution(SOLUTIONS *solutions, path_info *const current_path){
	long int j = EMPTY;
	for(long int i = ((long int) solutions->num_sol - UNITY); i >= EMPTY; i--)
		if ((int) solutions->paths[i]->geodesic_dis == (int) current_path->geodesic_dis && solutions->paths[i]->path_size == current_path->path_size){
			for(j = ((long int) current_path->path_size - UNITY); j >= EMPTY; j--)
				if (solutions->paths[i]->path_indexes[j] != current_path->path_indexes[j])
					break;
			if (j < EMPTY)
				return FALSE;
		}
	return TRUE;
};

//Used to free the memory from the 'current path' structure.
static void function_pathDestroy(path_info *current_path){
	if (current_path != NULL){
		if (current_path->path_indexes != NULL)
			free(current_path->path_indexes);
		free(current_path);
	}
}

//This functions makes a copy of a brand-new solution to a register, to be displayed later.
static void function_addSolution(SOLUTIONS *solutions, path_info *const current_path){
	//This function creates a copy of a solution because the solutions must be printed sorted
	if (solutions != NULL && current_path != NULL){
		//Check if the solution is not a copy of another one already registered
		if (function_validateSolution(solutions, current_path)){
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
	}
};

//This function init a solution data register
SOLUTIONS *function_solutionsInit(){
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

//Auxiliary function of pathfinding.
static CHAMBER *function_backtrack(MAZE *maze, STACK *tracking){
	//IMPORTANT: THIS IS >NOT< A RECURSIVE BACKTRACKING, it's just a iterative simulation.
	//This functions returns to a memorized chamber.

	//First, we get the top adress of tracking memory.
	CHAMBER *traveller = stack_pop(tracking), *aux_geodis = traveller;

	//We need to get back to the last memorized 'multi-ways' chamber.
	do{
		//Not gonna mark off dead-ends with no purpose.
		if (traveller->num_paths > UNITY || traveller->end_chamber)
			traveller->marked = FALSE;
		maze->current_path->path_size--;
		traveller = stack_pop(tracking);

		maze->current_path->geodesic_dis -= EUCLIDEAN(aux_geodis, traveller);
		aux_geodis = traveller;
		
		#ifdef DEBUG
			printf("D: (BT) traveller cur ind: [%hd] (%hd).\n", 
				traveller->index, traveller->num_paths - traveller->paths_minus);
		#endif
	} while(!stack_empty(tracking) && traveller != stack_top(maze->memory));
	return stack_pop(maze->memory);
};

//Main function of pathfinding.
void maze_searchPaths(MAZE *maze, SOLUTIONS *solutions){
	if (maze != NULL){
		STACK *tracking = stack_create();
		maze->current_path = function_pathInit();
		CHAMBER *traveller = maze->start;
		boolean FLAG = TRUE;
		if (tracking != NULL && maze->current_path != NULL){
			stack_push(tracking, traveller);
			traveller = traveller->paths[EMPTY];
			while(FLAG){
					//Geodesic distance information...
				CHAMBER *aux_geodis = traveller;
					//It's also very important to stack the current chamber on the auxiliary stack memory too
				stack_push(tracking, traveller);
					//Debug information:
					#ifdef DEBUG
						printf("D: (GO) traveller cur ind: [%hd] (%hd).\n", 
						traveller->index, traveller->num_paths - traveller->paths_minus);
					#endif
					//Last but not least, check if the current chamber has a maze exit and this solutions is not a backtrack copy
				if (!traveller->marked){
						//First things first current chamber to the currenth path solution
					maze->current_path->path_indexes = (short int *) realloc(maze->current_path->path_indexes, 
						sizeof(short int) * (UNITY + maze->current_path->path_size));
					maze->current_path->path_indexes[maze->current_path->path_size++] = traveller->index;
					if (traveller->end_chamber){
							//If true, register the current solution.
						function_addSolution(solutions, maze->current_path);
					}
				}
					//mark the current chamber is a must 
				traveller->marked = TRUE;
					//Now we going to see all the possibilities of the current chamber.
				if(traveller->index == INVALID || traveller->num_paths == traveller->paths_minus){
						//These are condiditions for exception cases
					if (traveller->index != INVALID && !stack_empty(maze->memory))
						traveller = function_backtrack(maze, tracking);
					else
						FLAG = FALSE;
				} else if (/*traveller->num_paths - traveller->paths_minus > UNITY*/traveller->num_paths > 2){
						//'paths_minus' tracks the number of exits of the current chamber.
						//If the statement above is true, then we got a brand-new exit (the way back does not counts, hence '- UNITY')
						//It's important to known that every extra way in a start chamber is already considered brand-news possible exits.
						//First, stack this chamber to backtrack in future
					stack_push(maze->memory, traveller);
						//Then, select a any way.
					CHAMBER *aux = NULL;
					do aux = traveller->paths[traveller->paths_minus++];
					while ((traveller->paths_minus < traveller->num_paths) && aux->marked);
					//We chosed a way, so there will be a one way less in the next time we visit this chamber.
					//First, verify if it's really a valid choice:
					if (aux->marked){
						stack_pop(maze->memory);	
						if (!stack_empty(maze->memory)){
							//There's something in the maze's memory. It's time to a "backtracking".
							//To not mix things up and *uck everything we call a auxiliary function.
							//Renember that the current chamber is already in the top of tracking stack memory.
							traveller = function_backtrack(maze, tracking);
						}else{
							//We don't have anything in maze stack memory, so the process supposedly is completed.
							FLAG = FALSE;//End loops.
						}
					}else{
						traveller = aux;
						//Keep track of the travel geodesic distance
						maze->current_path->geodesic_dis += EUCLIDEAN(aux_geodis, traveller); 
					}
				}else{
						//The chamber does not have much options. We have two possibilities:
						//a) or this chamber is a dead-end
						//b) ''   ''  ''     has only one way left 
						//To be sure of this:
					if (traveller->num_paths == UNITY){
							//It's a dead end, i.e the only possible way out is the way we came from.
							//In this situation, we open two new possibilities:
							//a.1) We have something stored in maze stack memory, i.e. a new possible way is left back
							//b.2) We don't have anything in maze stack memory, so nothing is left back.
							//Then...
						if (!stack_empty(maze->memory)){
								//There's something in the maze's memory. It's time to a "backtracking".
								//To not mix things up and *uck everything we call a auxiliary function.
								//Renember that the current chamber is already in the top of tracking stack memory.
							traveller = function_backtrack(maze, tracking);
						}else{
								//We don't have anything in maze stack memory, so the process supposedly is completed.
								FLAG = FALSE;//End loops.
							}
						} else{
							//We are supposedly in a one-way track, i.e. the chamber has only the way back and one way to proceed.
							//Not a new thing, we have two options in this situation too:
							//b.1) Or we can proceed naturally.
							//b.2) Or not, because the only way left will get us back to somewhere we already passed by:
							// (Way back) <--- (Current chamber) ---> (Somewhere already marked)
							//So...
							if (!(traveller->paths[EMPTY]->marked & traveller->paths[UNITY]->marked)){
								//There's a natural way. Best choose the right one, not the way we come from.
								//Again, the current chamber is already marked and stacked on tracking memory.
								CHAMBER *aux = traveller->paths[EMPTY];
								if(aux->marked)
									aux = traveller->paths[UNITY];
								traveller = aux;
								//Keep track of the travel geodesic distance
								maze->current_path->geodesic_dis += EUCLIDEAN(aux_geodis, traveller);
							}else {
								//Both ways are marked. This situation is like a dead-end chamber. So...
								if (!stack_empty(maze->memory)){
									//There's something in the maze's memory. It's time to a "backtracking".
									//To not mix things up and *uck everything we call a auxiliary function.
									//Renember that the current chamber is already in the top of tracking stack memory.
									traveller = function_backtrack(maze, tracking);
								}else{
									//We don't have anything in maze stack memory, so the process supposedly is completed.
									FLAG = FALSE;//End loops.
								}
							}
						}
					}
				}
				stack_destroy(tracking);
				function_pathDestroy(maze->current_path);
				maze->current_path = NULL;
			}
		}
};

//Those four functions below are just to request some encapsuled information, from here to main.
//Request maze number of dots/chambers.
unsigned int get_numDots(MAZE *maze){
	if (maze != NULL)
		return maze->num_dots;
	return EMPTY;
};

//Request number of paths of a specific chamber 
unsigned int get_numPaths(CHAMBER **chambers, short int index){
	if (chambers != NULL && index >= EMPTY)
		return chambers[index]->num_paths;
	return EMPTY;
};

//Rotates the adresses of a specific chamber to the left
void function_swapAdress(CHAMBER **chambers, short int index){
	if (chambers != NULL && index >= EMPTY)
		for(short int i = EMPTY; i < (chambers[index]->num_paths - UNITY); i++)
			SWAP(chambers[index]->paths[i], chambers[index]->paths[i + UNITY]);
};

//Reset chambers to a new pathfinding.
void function_cleanChambers(CHAMBER **chambers, int size){
	if (chambers != NULL){
		for(int i = EMPTY; i < size; i++){
			chambers[i]->marked = FALSE;
			chambers[i]->paths_minus = EMPTY;
		}
		chambers[size]->paths_minus = EMPTY;
	}
};