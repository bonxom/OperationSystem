all: tinyshell

tinyshell: main.o shell.o process.o utils.o
	gcc -o tinyshell main.o shell.o process.o utils.o

main.o: main.c shell.h
	gcc -c main.c

shell.o: shell.c shell.h process.h utils.h
	gcc -c shell.c

process.o: process.c process.h utils.h
	gcc -c process.c

utils.o: utils.c utils.h
	gcc -c utils.c

clean:
	rm -f *.o tinyshell
