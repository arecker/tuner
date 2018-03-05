CFLAGS=-Wall -lasound -g -ldl -lm

all:
	gcc ${CFLAGS} -o tuner main.c
