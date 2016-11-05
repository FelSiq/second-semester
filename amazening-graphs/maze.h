#ifndef __MAZE_H_
#define __MAZE_H_

typedef struct maze MAZE;
typedef struct chamber CHAMBER;
typedef struct solutions SOLUTIONS;

SOLUTIONS *function_solutionsInit();
MAZE *maze_init(CHAMBER ***);
void maze_searchPaths(MAZE *, SOLUTIONS *);
void maze_destroy(MAZE **, CHAMBER ***);
void maze_solutionsSort(SOLUTIONS *);
void maze_solutionsPrint(SOLUTIONS *const);
void maze_solutionsDestroy(SOLUTIONS *);

unsigned int get_numDots(MAZE *);
unsigned int get_numPaths(CHAMBER **, short int);
void function_swapAdress(CHAMBER **, short int);
void function_cleanChambers(CHAMBER **, int);

#endif