CC     = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall
HWK    = /c/cs223/Hwk2

Count:  Psched.o
	${CC} ${CFLAGS} -o Psched Psched.o

Subst16: Subst16.o library.o
	$(CC) $(CFLAGS) $^ -o $@

Subst16.o library.o: library.h
