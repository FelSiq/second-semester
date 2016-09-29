all: vectors.o main.o
	gcc vectors.o main.o -o calculadora -lm

vectors.o: 
	gcc -c vectors.c
	
main.o: 
	gcc -c main.c

run:
	./calculadora
clean:
	rm *.o calculadora
