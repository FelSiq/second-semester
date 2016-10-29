#include <stdlib.h>
#include <stdio.h>
#include "sparse.h"
#include <time.h>

//Function for node memory allocation and variables initializing
NODE *function_nodeCreate(){
	NODE *node = (NODE *) malloc(sizeof(NODE));
	if (node != NULL){
		node->value = EMPTY;
		node->i_col = INVALID;
		node->i_row = INVALID;
		node->right = node;
		node->down = node;
	}
	return node;
}

//Auxiliar function that take care of the rows headnodes of matrix, i.e the headnodes on the -1 column
boolean function_rowSetup(MATRIX *matrix, unsigned int row){
	NODE *node = NULL;
	unsigned int i;
	if (matrix != NULL){
		for(i = EMPTY; i < row; i++){
			node = function_nodeCreate();
			if (node != NULL){
				node->i_row = (row - UNITY - i);
				node->down = matrix->node_master->down;
				matrix->node_master->down = node;
			} else break;
		}
		if (i == row)
			return TRUE;
	}
	return FALSE;
}

//Reciprocally to the columns headnodes, that are on the -1 matrix line
boolean function_colSetup(MATRIX *matrix, unsigned int col){
	NODE *node = NULL;
	unsigned int j;
	if (matrix != NULL){
		for(j = EMPTY; j < col; j++){
			node = function_nodeCreate();
			if (node != NULL){
				node->i_col = (col - UNITY - j);
				node->right = matrix->node_master->right;
				matrix->node_master->right = node;
			} else break;
		}
		if (j == col)
			return TRUE;
	}
	return FALSE;
}

//This functions create the base of a sparse matrix with simple-connected nodes
MATRIX *matrix_create(unsigned int row, unsigned int col){
	MATRIX *matrix = NULL;
	if (row > EMPTY && col > EMPTY){
		matrix = (MATRIX *) malloc(sizeof(MATRIX));
		if (matrix != NULL){
			matrix->row_num = row;
			matrix->col_num = col;
			matrix->node_master = function_nodeCreate();
			if (matrix->node_master != NULL && function_rowSetup(matrix, row) && function_colSetup(matrix, col))
				return matrix;
		}
	}
	matrix_destroy(&matrix);
	return NULL;
}

//And that is responsable for all the memory deallocation of a future inexistent sparse matrix
boolean matrix_destroy(MATRIX **matrix){
	NODE *aux_col, *aux_row, *aux_delete;
	if(matrix != NULL && *matrix != NULL){
		aux_col = (*matrix)->node_master->right;
		if (aux_col != NULL){
			//Item mass deletion
			while(aux_col != (*matrix)->node_master){
				aux_row = aux_col->down;
				while(aux_row != aux_col){
					aux_delete = aux_row;
					aux_row = aux_row->down;
					free(aux_delete);
					aux_delete = NULL;
				}
				aux_row = NULL;
				aux_col = aux_col->right;
			}
			//Now we got only the head nodes, aux_col -> mnode_master
			//Deleting the headnodes column 
			aux_row = (*matrix)->node_master->down;
			while(aux_row != (*matrix)->node_master){
				aux_delete = aux_row;
				aux_row = aux_row->down;
				free(aux_delete);
				aux_delete = NULL;
			}
			//Now deleting the headnodes line
			aux_col = (*matrix)->node_master->right;
			while(aux_col != (*matrix)->node_master){
				aux_delete = aux_col;
				aux_col = aux_col->right;
				free(aux_delete);
				aux_delete = NULL;
			}
			//last but not least, freeing the (-1,-1) of matrix
			free((*matrix)->node_master);
		}
		free(*matrix);
		(*matrix) = NULL;
		return TRUE;
	}
	return FALSE;
}

//One of the most complicated functions of the code: to add a new already existent node to a sparse matrix
boolean function_addNode(MATRIX *matrix, NODE *node){
	NODE *aux = NULL, *aux_prev = NULL, *aux_marker = NULL;
	if (matrix != NULL && node != NULL && node->value != EMPTY){
		//Renember that empty values will not be stored in sparse matrix
		aux = matrix->node_master->right;
		if (aux != NULL){
			//First, i got the right column to add teh new element
			while(aux != matrix->node_master && aux->i_col != node->i_col)
				aux = aux->right;
			//Now, we need to search for the right line
			//first things first, i need to setup some poiters to things works well:
			aux_prev = aux; //This gonna be the bridge to reach where my new node gonna be set
			aux_marker = aux; //to warn me if a reach the end of the column.
			aux = aux->down;
			//Digging.
			while(aux != aux_marker && aux->i_row < node->i_row){
				aux_prev = aux;
				aux = aux->down;
			}

			//By now, I need to check if the element already exists. If true, i need to cancel everything and return an error.
			if (aux->i_row == node->i_row)
				return FALSE;

			//Ok. Supposedly reached the right place i.e. aux_prev->down.
			node->down = aux_prev->down;
			aux_prev->down = node;
			//Now i need to connect the new node to its correct line.
			//example:
			//|[-1,-1][-1, 1][-1, 2][-1, 3][-1, 4]	|	will add on |	[-1, 0][-1, 1][-1, 2][-1, 3][-1, 4]	|
			//|[ 0,-1]---\/-----|-------|->[ 0, 4]	|	[ 1, 3]		|	[ 0,-1]---\/-----|-----\\//>[ 0, 4]	|
			//|[ 1,-1][ 1, 1]	|		\/	  |		|				|==>[ 1,-1][ 1, 1]======>[ 1, 3]========|
			//|[ 2,-1]---------\/-->[ 2, 3]	  |		| 	=======>	|	[ 2,-1]---------\/-->[ 2, 3]   |	|
			//|[ 3,-1]------>[ 3, 2][ 3, 3]	  \/	|	 (new)		|	[ 3,-1]------>[ 3, 2][ 3, 3]   \/	|
			//|[ 4,-1]-------------------->[ 4, 4]	|				|	[ 4,-1]-------------------->[ 4, 4]	|
			//Please note that circular pointers (except for the new one) are omitted in that example.
			aux = matrix->node_master->down;
			while(aux != matrix->node_master && aux->i_row != node->i_row)
				aux = aux->down;
			//Supposedly reached the node's correct line, now i need to set some new pointers setup
			aux_marker = aux;
			aux_prev = aux;
			aux = aux->right;

			while(aux != aux_marker && aux->i_col < node->i_col){
				aux_prev = aux;
				aux = aux->right;
			}
			//Founded the right place, i.e aux_prev->right
			node->right = aux_prev->right;
			aux_prev->right = node;
			//Hopefully all things gonna work now.
			return TRUE;
		}
	}
	return FALSE;
}

//This function receive some data inputs and transform it to possible new nodes on a sparse matrix
//I needed to create a anti-infinite loop condition (FLAG_STOP) because there's no solid stop condition
//on the test inputs.
boolean matrix_fill(MATRIX *matrix, boolean *FLAG_NEWMAT){
	int row = EMPTY, col = EMPTY, val = EMPTY;
	NODE *new_node = NULL;
	boolean FLAG_ERROR = TRUE, FLAG_STOP = FALSE;
	clock_t ck_start = clock(), ck_end = EMPTY;
	int junk;

	if (matrix != NULL){
		while(!FLAG_STOP && (ck_end - ck_start)/ (CLOCKS_PER_SEC * 1.0) < 1.9){
			//This will abort the filling process if takes too long.
			ck_end = clock();

			junk = scanf("%d", &row);
			if (row != INVALID){
				junk = scanf("%d%d", &col, &val);
				getchar();
				//if (val != EMPTY){
					new_node = function_nodeCreate();
					if (new_node != NULL){
						FLAG_ERROR = FALSE;
						new_node->value = val;
						new_node->i_col = col;
						new_node->i_row = row;
						if(!function_addNode(matrix, new_node)){
							free(new_node);
							FLAG_STOP = TRUE;
						}
					}
				//}
			} else break;
		} 
		if (row == INVALID)
			(*FLAG_NEWMAT) = TRUE;
	}
	//debug information for total time taken by this process:
	//printf("test clock: %lf\n", (ck_end - ck_start)/ (CLOCKS_PER_SEC * 1.0));
	return !(FLAG_ERROR);
}

//Not used on the final version of the program, but still kept here because it's a very powerful debug function.
boolean matrix_nodeCount(MATRIX *matrix){
	//this is totally a debug function
	NODE *aux = NULL, *aux_marker;
	unsigned int counter = EMPTY;
	if (matrix != NULL){
		aux = matrix->node_master;
		if (aux != NULL){
			//first, i will count line-by-line
			aux = aux->down;
			while(aux != matrix->node_master){
				aux_marker = aux;
				aux = aux->right;
				while(aux != aux_marker){
					aux = aux->right;
					counter++;
				}
				aux = aux->down;
			}
			printf("line-by-line result: %u.\n", counter);
			counter = EMPTY;
			//now i will count column-by-column and, of course, the final counter need to be 
			//the same, or something went wrong in matrix construction
			aux = aux->right;
			while(aux != matrix->node_master){
				aux_marker = aux;
				aux = aux->down;
				while(aux != aux_marker){
					aux = aux->down;
					counter++;
				}
				aux = aux->right;
			}
			printf("column-by-column result: %u.\n", counter);
			return TRUE;
		}
	}
	return FALSE;
}

//Probably the most or 2th most complicated function of the program. Maybe I put excessive comentaries on, but totally worth it.
MATRIX *matrix_opAdd(MATRIX *matrix_a, MATRIX *matrix_b){
	//In this function I will be using propositally larger names, just to maintain the concentration of what i'm doing.
	NODE *node_marker_a = NULL, *node_marker_b = NULL, *node_travel_a = NULL, *node_travel_b = NULL;
	MATRIX *matrix_r = NULL;
	boolean FLAG_MOVE_A = FALSE;

	if(matrix_a != NULL && matrix_b != NULL){
		//setup important things first
		matrix_r = matrix_create(matrix_a->row_num, matrix_a->col_num);
		//ok. let's think for a moment
		//A(4, 5)/B(4, 5)
		//					A:													B:												 R:
		//[-1,-1][-1, 0][-1, 1][-1, 2][-1, 3][-1, 4] 		[-1,-1][-1, 0][-1, 1][-1, 2][-1, 3][-1, 4]		[-1,-1][-1, 0][-1, 1][-1, 2][-1, 3][-1, 4] 
		//[ 0,-1][ 0, 0]			  [ 0, 3]				[ 0,-1][ 0, 0]		 [ 0, 2]	   [ 0, 4]		[ 0,-1][ 0, 0]		 [ 0, 2][ 0, 3][ 0, 4]
		//[ 1,-1]			   		  [ 1, 3]			+	[ 1,-1]		  [ 1, 1]						=	[ 1,-1]		  [ 0, 1]		[ 1, 3]
		//[ 2,-1][ 2, 0]			  [ 2, 3]				[ 2,-1]              [ 2, 2][ 2, 3]				[ 2,-1][ 2, 0]		 [ 2, 2][ 2, 3]
		//[ 3,-1]		[ 3, 1][ 3, 2]						[ 3,-1][ 3, 0][ 3, 1]							[ 3,-1][ 3, 0][ 3, 1][ 3, 2]
		//At first, I have two options: do a sum line-to-line and then changing columns or vice-versa. There's no pratical diference about it in general.
		//Then, I will do a sum column-to-column and changing lines as columns ends.
		//I need four pointers to do that: two for travelling the A and B matrixes plus two to mark the end-of-line 
		//(these two are not really necessary but it can do things go faster a little bit, what is very relevant especially in this case).
		//There's three possible cases in matrixes sum:
		//	> There's elements (non nulls) on both matrixes: then, in matrix R[x, y] must be a A[x, y] + B[x, y], and both travel pointers proceed
		//		-> verification condition: A[x,y]->i_col == B[x, y]->i_col (renember I am travelling column-to-column before changing lines)
		//	> There's element only on matrix A: then, in matrix R[x, y] must be a copy of A[x, y]
		//		-> verification condition: A[x,y]->i_col != B[x, y]->i_col
		//	> Reciprocally, R[x, y] must be a copy of B[x, y] when A[x, y] is a non-existent node. Ver. condition is the same.
		//	Independent of the case, all travel pointers MUST stop when:
		//		-> pointer_travel_x == pointer_marker_x
		if (matrix_r != NULL){
			//To start a column-to-column travel, the extern loop must be for lines, and then loop for columns
			node_travel_a = matrix_a->node_master;
			node_travel_b = matrix_b->node_master;
			if (node_travel_a != NULL && node_travel_b != NULL){
				node_travel_a = node_travel_a->down;
				node_travel_b = node_travel_b->down;
				//Both travels pointers now are on [0, -1] of theirs respectives matrixes
				while(node_travel_a != matrix_a->node_master || node_travel_b != matrix_b->node_master){
					//This loops changes lines
					node_marker_a = node_travel_a; //	markers
					node_marker_b = node_travel_b; // 	setup
					node_travel_a = node_travel_a->right; //	Initial pull for
					node_travel_b = node_travel_b->right; //	travel pointers
					//Now, both marker pointers are on [x, -1] and both travel pointers on [x, 0] of their respectives matrixes. 
					while(node_travel_a != node_marker_a || node_travel_b != node_marker_b){
						//This loops changes columns
						//I can already create a new node because I already known that al least one of travel pointers don't reached the line end.
						NODE *new_result_node = function_nodeCreate();
						if (new_result_node != NULL){
							if (node_travel_a != node_marker_a){
								//It's important to check priority (i.e. or B matrix line reached the end already or 
								//is A[x,y]->i_col smaller or equal to B[x,y]->i_col) before changing the new node's value.
								if (node_travel_b->i_col == INVALID || node_travel_a->i_col <= node_travel_b->i_col){
									new_result_node->value = node_travel_a->value; //+= not really necessary in this case.
									new_result_node->i_col = node_travel_a->i_col;
									new_result_node->i_row = node_travel_a->i_row;
									//I can only make a step with pointer traveller A if it's ina dif pos in relation of traveller pointer B
									//because (*) condition below. Plus, it's not a good idead to move A before checking B, then sadly
									//I needed to use a flag for that.
									if (node_travel_a->i_col != node_travel_b->i_col)
										FLAG_MOVE_A = TRUE;
								}
							}
							if (node_travel_b != node_marker_b){
								//now reciprocally to B and so on.
								if (node_travel_a->i_col == INVALID || node_travel_b->i_col <= node_travel_a->i_col){//(*)
									new_result_node->value += node_travel_b->value; // note that in this case += is a MUST.
									new_result_node->i_col = node_travel_b->i_col;
									new_result_node->i_row = node_travel_b->i_row;
									//now, this time maybe i need to move A and B pointers together
									if (node_travel_a->i_col == node_travel_b->i_col)
										FLAG_MOVE_A = TRUE;	
									node_travel_b = node_travel_b->right;
								}
							}
							//FLAG to move or don't move travel pointer A. Maybe there's a way to remove this?!
							if (FLAG_MOVE_A){
								node_travel_a = node_travel_a->right;
								FLAG_MOVE_A = FALSE;
							}
						}
						//Then, after everything, add the new result node to result matrix. My 'add' function already check if node's value is empty.
						//In case of errors like this, node is automatically destroyed.
						if(!function_addNode(matrix_r, new_result_node))
							free(new_result_node);				
						//Intern (columns) loop ends here.
					}
					//it's importante that now both travel pointers are back to the [x, -1] position, like their respectives marker pointers.
					//moving to the next line...
					node_travel_a = node_travel_a->down;
					node_travel_b = node_travel_b->down;
					//Extern (lines) loop ends here.
				}
			}
		}
	}
	return matrix_r;
}

//Another opration function. 
MATRIX *matrix_opMult(MATRIX *matrix_a, MATRIX *matrix_b){
	NODE *node_travel_a = NULL, *node_travel_b = NULL, *node_marker_a = NULL, *node_marker_b = NULL;
	MATRIX *matrix_r = NULL;
	boolean FLAG_MOVE_A = FALSE;
	if(matrix_a != NULL && matrix_b != NULL){
		matrix_r = matrix_create(matrix_a->row_num, matrix_b->col_num);
		if (matrix_r != NULL){
			//Okay, first things first, let's think about matrixes multiplication
			//At moment, I guaranteed that the number of columns in A is equal to the number of lines in B, so the multiplication is valid.
			//The matrix resultant is already prepared at this point.
			//I know that a node will only be created on reault matrix if there's at least one match of a pair of elements like A[x,y] and B[y,x].
			//So, let's see a example:
			//A(2,3)/B(3,4)
			//				A:											B:											R:
			//[-1,-1][-1, 0][-1, 1][-1, 2]			[-1,-1][-1, 0][-1, 1][-1, 2][-1, 3]			[-1,-1][-1, 0][-1, 1][-1, 2][-1, 3]
			//[ 0,-1]		[-1,-1]			 x 		[ 0,-1]		  [-1,-1]		[-1,-1]		=	[ 0,-1]		  [-1,-1]
			//[ 1,-1]			   [-1,-1]			[ 1,-1]		  [-1,-1][-1,-1]				[ 1,-1]
			//										[ 2,-1][-1,-1]	
			node_travel_a = matrix_a->node_master;
			node_travel_b = matrix_b->node_master;
			if (node_travel_a != NULL && node_travel_b != NULL){
				//At this point, both traveller nodes are on [-1,-1] i.e. master node of its respectives matrixes.
				//Node_travel_a will be fixed in a single line and switch between matrix A columns, and node_travel_b will be
				//fixed on a single column and switch between matrix B lines, until its end.
				node_travel_a = node_travel_a->down;
				node_travel_b = node_travel_b->right;
				//node_travel_a is now on [x,-1] and node_travel_b is on [-1, x].
				while(node_travel_a != matrix_a->node_master){
					node_marker_a = node_travel_a;
					
					while(node_travel_b->i_col != INVALID){
						//Just creating a possible new node for every suboperation
						NODE *new_result_node = function_nodeCreate();
						if(new_result_node != NULL){
							node_travel_a = node_travel_a->right;
							node_marker_b = node_travel_b; //Node marker B must be constantly atuallized
							node_travel_b = node_travel_b->down;
							//now the traveller nodes are, on A[x,0] and B[0,x].
							//This loop goes on until the traveller pointer B reaches the matrix B current line
							while(node_travel_a != node_marker_a && node_marker_b != node_travel_b){
								//There's no need to make a verification if one traveller node already reached the dead end, because the result will be always null.
								if (node_travel_a->i_col == node_travel_b->i_row){
									new_result_node->value += (node_travel_a->value * node_travel_b->value); //This is a sum so += is a must
									new_result_node->i_row = node_marker_a->i_row;
									new_result_node->i_col = node_marker_b->i_col;
								}
								//Now i need to move travel pointers. Again, thats not a good idea move's A before (*) condition, thus the use of a flag.
								if(node_travel_a->i_col <= node_travel_b->i_row)
									FLAG_MOVE_A = TRUE;
								if (node_travel_b->i_row <= node_travel_a->i_col) //(*)
									node_travel_b = node_travel_b->down;
								//Maybe not the best solution, but still works well.
								if (FLAG_MOVE_A){
									FLAG_MOVE_A = FALSE;
									node_travel_a = node_travel_a->right;
								}
								//Loop for A lines x B column multiplication ends here.
							}
							//Preparing travel pointers to the next step.
							node_travel_a = node_marker_a;
							node_travel_b = node_marker_b->right;
							//At end, try to add new element of result matrix. If something went wrong, deallocate nodes memory.
							if(!function_addNode(matrix_r, new_result_node))
								free(new_result_node);
						}
						//Loop for B's lines ends here.
					}
					//Node_travel_a is now on [x, -1], then it's going to the next A's line.
					node_travel_a = node_travel_a->down;
					node_travel_b = matrix_b->node_master->right;
					//Loop for matrix A lines ends here.
				}
			}
		}
	}
	return matrix_r;
}

//Used to print all non-null elements of a sparse matrix, alongside its coordinates i.e. [x][y][value] from M[x][y] of a sparse matrix M.
boolean matrix_print(MATRIX *matrix){
	NODE *aux = NULL, *aux_marker = NULL;
	if (matrix != NULL){
		printf("-1 %u %u\n", matrix->row_num, matrix->col_num);
		//i need to walk on every possible existent element of matrix
		aux = matrix->node_master;
		if (aux != NULL){
			aux = aux->down;
			while(aux != matrix->node_master){
				aux_marker = aux;
				aux = aux->right;
				while(aux != aux_marker){
					printf("%u %u %d\n", aux->i_row, aux->i_col, aux->value);
					aux = aux->right;
				}
				aux = aux->down;
			}
			return TRUE;
		}
	}
	return FALSE;
}