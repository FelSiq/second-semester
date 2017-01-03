all: stack.o bin_t.o arith.o main.o
	gcc stack.o bin_t.o arith.o main.o -o arith -Wall
stack.o:
	gcc -c stack.c -Wall
bin_t.o:
	gcc -c bin_t.c -Wall
arith.o:
	gcc -c arith.c -Wall
main.o:
	gcc -c main.c -Wall
clean:
	rm *.o arith
run:
	./arith