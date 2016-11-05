all: stack.o maze.o main.o
	gcc stack.o maze.o main.o -o maze -lm -Wall
stack.o:
	gcc -c -g stack.c -Wall
maze.o:
	gcc -c -g maze.c -Wall -lm
main.o:
	gcc -c -g main.c -Wall
clean:
	rm *.o maze
run:
	./maze