all: bwlist.o main.o
	gcc bwlist.o main.o -o bwlist -Wall
	
bwlist.o:
	gcc -c bwlist.c

main.o:
	gcc -c main.c
	
clean:
	rm *.o bwlist
	
run:
	./bwlist