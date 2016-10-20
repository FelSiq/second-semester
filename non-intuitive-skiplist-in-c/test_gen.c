#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[]){
	unsigned long long int i = 100000;
	clock_t clk;
	char str0[200], str1[200];
	while(i-- > 0){
		clk = (clock()/((CLOCKS_PER_SEC/1000) * 1.0) * 100);
		srand(time(NULL) + clk);
		sprintf(str0, "%u", rand());
		clk *= 3;
		srand(time(NULL) + clk);
		sprintf(str1, "%u", rand());
		printf("i %s %s\n", str0, str1);
	}
	return 0;
}