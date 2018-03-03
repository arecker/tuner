CFLAGS=-Wall -lasound -g

all:
	gcc ${CFLAGS} -o tuner main.c
