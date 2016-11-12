all: clean utils.o site.o command.o googlebot.o main.o
	gcc utils.o site.o command.o googlebot.o main.o -o miniGoogle -Wall

utils.o:
	gcc -c utils.c

site.o:
	gcc -c site.c
	
command.o:
	gcc -c command.c
	
googlebot.o:
	gcc -c googlebot.c
	
main.o:
	gcc -c main.c
	
clean:
	rm *.o miniGoogle