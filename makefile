all:
	make control phone

control: control.o
	gcc -o control control.o
	make clean

phone: phone.o
	gcc -o phone phone.o
	make clean

phone.o: phone.c consts.h
	gcc -c phone.c consts.h

control.o: control.c consts.h
	gcc -c control.c consts.h

clean:
	rm *.o

