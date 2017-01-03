#include <stdlib.h>
#include <stdio.h>
#include "arith.h"
#include "utils.h"
#include "bin_t.h"

int main(int argc, char const *argv[]){
	BIN_T *bin_t = bint_init();
	if (bin_t != NULL){
		get_entries(bin_t);
		bint_print(bin_t);
		bint_solve(bin_t);
		printf("\n");
		bint_destroy(&bin_t);
	}
	return 0;
}