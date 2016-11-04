all: stack.o maze.o main.o
	gcc stack.o maze.o main.o -o maze -lm -Wall -DDEBUG
stack.o:
	gcc -c -g stack.c -Wall -DDEBUG
maze.o:
	gcc -c -g maze.c -Wall -lm -DDEBUG
main.o:
	gcc -c -g main.c -Wall -DDEBUG
clean:
	rm *.o maze
run:
	./maze