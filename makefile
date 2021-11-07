all: hw-13.o
	gcc -o program hw-13.o

hw-13.o: hw-13.c
	gcc -c hw-13.c