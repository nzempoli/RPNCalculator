all: calc

CFLAGS=-g -Og -Wall -std=c99
CC=gcc

calc: calc.c rpn.c stack.c token.c hash.c node.c symbol.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f calc
