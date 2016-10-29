all: data.o sparse.o main.o
	@gcc data.o sparse.o main.o -o sparsemat -Wall
data.o:
	@gcc -c -O3 data.c
sparse.o:
	@gcc -c -O3 sparse.c
main.o:
	@gcc -c -O3 main.c
run:
	@./sparsemat
clean:
	rm *.o sparsemat