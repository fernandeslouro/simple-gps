#makefile prog

all: gps2013

main.o: main.c funcoes.h
	gcc -c -g -ansi -pedantic -Wall main.c -lm -lg2

funcoes.o:funcoes.c funcoes.h
	gcc -c -g -ansi -pedantic -Wall funcoes.c -lm -lg2


gps2013: funcoes.o main.o
	gcc -g -ansi -pedantic -Wall funcoes.o main.o  -lm -lg2 -o gps

clean:
	rm -r funcoes.o main.o
