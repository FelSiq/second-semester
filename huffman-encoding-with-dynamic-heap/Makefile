all: deque.o dynheap.o huff.o trab4.o
	gcc deque.o dynheap.o huff.o trab4.o -o huff -Wall
deque.o:
	gcc -c deque.c -Wall
dynheap.o:
	gcc -c dynheap.c -Wall
huff.o:
	gcc -c huff.c -Wall
trab4.o:
	gcc -c trab4.c -Wall
clean:
	rm *.o huff
run:
	./huff