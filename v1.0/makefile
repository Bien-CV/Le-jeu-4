all : Minimale

Minimale : Minimale.o liste.o file.o
	gcc -o Minimale Minimale.o liste.o file.o
	
Minimale.o : Minimale.c 
	gcc -c Minimale.c

liste.o : liste.c 
	gcc -c liste.c liste.h

file.o : file.c 
	gcc -c file.c file.h

clean :
	rm Minimale.o Minimale core
