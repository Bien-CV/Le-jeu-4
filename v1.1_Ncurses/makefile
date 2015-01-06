

PROG=Minimale
OBJS=liste.o file.o pile.o
OPTS=-Wall

all : ${PROG}

${PROG} : ${OBJS}
	${CC} -o ${PROG} ${PROG}.c ${OBJS} ${OPTS} -lncurses
	
liste.o : liste.c 
	${CC} -c liste.c liste.h ${OPTS}

file.o : file.c 
	${CC} -c file.c file.h ${OPTS}

pile.o : pile.c
	${CC} -c pile.c pile.h ${OPTS}

clean :
	rm *.o ${PROG}
