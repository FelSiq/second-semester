all: stack.o maze.o main.o
	gcc stack.o maze.o main.o -o maze -lm -Wall -DDEBUG
stack.o:
	gcc -c stack.c -Wall -DDEBUG
maze.o:
	gcc -c maze.c -Wall -lm -DDEBUG
main.o:
	gcc -c main.c -Wall -DDEBUG
clean:
	rm *.o maze
run:
	./maze