all: tinyshell

tinyshell: main.o shell.o process.o utils.o display.o
	gcc -o tinyshell main.o shell.o process.o utils.o display.o

main.o: main.c shell.h display.h
	gcc -c main.c display.c

shell.o: shell.c shell.h process.h utils.h
	gcc -c shell.c

process.o: process.c process.h utils.h
	gcc -c process.c

utils.o: utils.c utils.h
	gcc -c utils.c
display.o: display.c display.h
	gcc -c display.c

clean:
	rm -f *.o tinyshell
