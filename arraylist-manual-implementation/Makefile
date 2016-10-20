all:  item.o utils.o commands.o arraylist.o main.o
	@gcc item.o utils.o commands.o arraylist.o main.o -o arraylist -Wall

item.o:
	@gcc -c item.c

utils.o:
	@gcc -c utils.c

commands.o:
	@gcc -c commands.c
	
arraylist.o:
	@gcc -c arraylist.c

main.o:
	@gcc -c main.c
	
clean:
	rm *.o arraylist
	
run:
	@./arraylist