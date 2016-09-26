all: register.o main.o compile
compile:
	@gcc register.o main.o -o registers -Wall
register.o: register.c utils.h
	@gcc -c register.c
main.o:
	@gcc -c main.c
clean:
	rm *.o registers
run:
	@./registers
