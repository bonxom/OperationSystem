all: tinyshell

tinyshell: main.o shell.o process.o utils.o commands.o
	gcc -o tinyshell main.o shell.o process.o utils.o commands.o -lreadline

main.o: main.c shell.h process.h
	gcc -c main.c

shell.o: shell.c shell.h process.h utils.h
	gcc -c shell.c

process.o: process.c process.h utils.h commands.h
	gcc -c process.c

utils.o: utils.c utils.h process.h
	gcc -c utils.c

commands.o: commands.c commands.h shell.h process.h utils.h
	gcc -c commands.c

clean:
	rm -f *.o tinyshell