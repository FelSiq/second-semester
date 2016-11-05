#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

/*
	NOTA: ESTE TRABALHO NÃO PRECISA SER CORRIGIDO POIS >NÃO< SOU DA TURMA.
*/

int main(int argc, char const *argv[]){
	CHAMBER **chambers = NULL;
	MAZE *maze = maze_init(&chambers);
	if (maze != NULL){
		//This structures kept all the registers of solutions founded in pathfinding.
		SOLUTIONS *solutions = function_solutionsInit();

		//First call of the pathfinding.
		maze_searchPaths(maze, solutions);

		//This loops is like a 'brute-force' pathfinding: it's try to get new 
		//possibilities for every combination of preferences in the chambers exits.
		//Note that only chambers with extras paths are taken into count.
		for(int i = ((int) get_numDots(maze) - UNITY); i >= EMPTY; i--){
			if (get_numPaths(chambers, i) > 2){
				function_cleanChambers(chambers, get_numDots(maze));
				for(int j = ((int) get_numPaths(chambers, i)); j >= EMPTY; j--){
					function_swapAdress(chambers, i);
					maze_searchPaths(maze, solutions);
				}
				//Set the adresses once again to get then in the original form
				function_swapAdress(chambers, i);
			}
		}
		//Now it's time to sort solutions and show then to the user.
		if (solutions != NULL){
			maze_solutionsSort(solutions);
			maze_solutionsPrint(solutions);
			maze_solutionsDestroy(solutions);
		}
		//And, at end, free all the memory left in maze and chambers structure.
		maze_destroy(&maze, &chambers);
	}
	return 0;
}