phone: main.o
	gcc -o phone main.o

main.o: main.c
	gcc -c main.c

