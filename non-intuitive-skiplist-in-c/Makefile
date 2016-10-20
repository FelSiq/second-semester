all: item.o utils.o skiplist.o main.o
	gcc item.o utils.o skiplist.o main.o -o skiplist -Wall
item.o:
	gcc -c item.c
utils.o:
	gcc -c utils.c
skiplist.o:
	gcc -c skiplist.c
main.o:
	gcc -c main.c
clean:
	rm *.o skiplist