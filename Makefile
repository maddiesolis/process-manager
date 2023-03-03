########################################################
# Makefile
# Description: Compiles necessary files for running PMan
########################################################
all: pman inf args

pman: pman.o list.o
	gcc -o pman pman.o list.o

pman.o: pman.c list.h
	gcc -c pman.c

list.o: list.c list.h
	gcc -c list.c

inf: inf.c
	gcc inf.c -o inf

args: args.c
	gcc args.c -o args

clean:
	-rm -rf *.o *.exe *.bak