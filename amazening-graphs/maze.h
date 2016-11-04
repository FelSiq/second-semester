#ifndef __MAZE_H_
#define __MAZE_H_

typedef struct maze MAZE;
typedef struct chamber CHAMBER;
typedef struct solutions SOLUTIONS;

MAZE *maze_init(CHAMBER ***);
void maze_destroy(MAZE **, CHAMBER ***);
SOLUTIONS *maze_searchPaths(MAZE *);
void maze_solutionsSort(SOLUTIONS *);
void maze_solutionsPrint(SOLUTIONS *const);
void maze_solutionsDestroy(SOLUTIONS *);

#endif