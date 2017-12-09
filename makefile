all:
	make control phone

control: control.o
	gcc -o control control.o
	make clean

phone: phone.o
	gcc -o phone phone.o
	make clean

phone.o: phone.c include/consts.h
	gcc -c phone.c include/consts.h

control.o: control.c include/consts.h
	gcc -c control.c include/consts.h

clean:
	rm *.o

