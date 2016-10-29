all:  grep.o main.o
	gcc grep.o main.o -o grep -Wall
grep.o:
	gcc -c grep.c
main.o:
	gcc -c main.c
run:
	./grep
clean:
	rm *.o grep