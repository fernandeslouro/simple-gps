#makefile prog

all: gps2013

main.o: main.c gps_functions.h
	gcc -c -g -ansi -pedantic -Wall main.c -lm -lg2 -lX11 

gps_functions.o:gps_functions.c gps_functions.h
	gcc -c -g -ansi -pedantic -Wall gps_functions.c -lm -lg2 -lX11 

gps2013: gps_functions.o main.o
	gcc -g -ansi -pedantic -Wall gps_functions.o main.o -lm -lg2 -lX11 -o gps

clean:
	rm -r gps_functions.o main.o
