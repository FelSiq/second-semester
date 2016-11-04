#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int main(int argc, char const *argv[]){
	CHAMBER **chambers = NULL;
	MAZE *maze = maze_init(&chambers);
	if (maze != NULL){
		SOLUTIONS *solutions = maze_searchPaths(maze, chambers);
		if (solutions != NULL){
			maze_solutionsSort(solutions);
			maze_solutionsPrint(solutions);
			maze_solutionsDestroy(solutions);
		}
		maze_destroy(&maze, &chambers);
	}
	return 0;
}