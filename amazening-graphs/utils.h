#ifndef __UTILS_H_
#define __UTILS_H_

#include <math.h>
#define EUCLIDEAN(A,B) sqrt(pow(A->cd_x - B->cd_x, 2) + pow(A->cd_y - B->cd_y, 2));
#define SWAP(A,B) {void *p = A; A = B; B = p;}

typedef enum{
	FALSE,
	TRUE
} boolean; 

enum{
	INVALID = -1,
	EMPTY,
	UNITY
};

enum{
	ENTER = 10,
	C_RETURN = 13,
	SPACE_BAR = 32
};


#endif