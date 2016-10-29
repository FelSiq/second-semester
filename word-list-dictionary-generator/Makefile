all: clean dictionary.o main.o
	gcc dictionary.o main.o -o dictionary -Wall

dictionary.o:
	gcc -c dictionary.c

main.o:
	gcc -c main.c
	
clean:
	rm *.o dictionary