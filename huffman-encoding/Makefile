all: huffman.o main.o
	gcc huffman.o main.o -o huff -Wall
huffman.o:
	gcc -c huffman.c -Wall -DDEBUG
main.o:
	gcc -c main.c -Wall
clean:
	rm *.o huff
