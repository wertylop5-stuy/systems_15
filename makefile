control: control.o
	gcc -o control control.o

phone: phone.o
	gcc -o phone phone.o

phone.o: phone.c
	gcc -c phone.c

control.o: control.c
	gcc -c control.c

