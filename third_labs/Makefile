# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread
LDFLAGS = -pthread -lrt

all: Parent Child

Parent: Parent.c
	$(CC) $(CFLAGS) -o Parent Parent.c $(LDFLAGS)

Child: Child.c
	$(CC) $(CFLAGS) -o Child Child.c $(LDFLAGS)

clean:
	rm -f parent child *.o
