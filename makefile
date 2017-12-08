all:
	make control phone

control: control.o
	gcc -o control control.o
	make clean

phone: phone.o
	gcc -o phone phone.o
	make clean

phone.o: phone.c
	gcc -c phone.c

control.o: control.c
	gcc -c control.c

clean:
	rm *.o

