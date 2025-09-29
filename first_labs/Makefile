CC = gcc
CFLAGS = -I./include
LDFLAGS = -lm
TARGET = Parent Child

.PHONY: all clean

all: $(TARGET)

Custom.o: src/Custom.c include/Custom.h
	$(CC) -c -o Custom.o src/Custom.c $(CFLAGS)

Child.o: Child.c include/Custom.h
	$(CC) -c -o Child.o Child.c $(CFLAGS)

Parent.o: Parent.c include/Custom.h
	$(CC) -c -o Parent.o Parent.c $(CFLAGS)

Parent: Parent.o Custom.o
	$(CC) -o Parent Parent.o Custom.o

Child: Child.o Custom.o
	$(CC) -o Child Child.o Custom.o $(LDFLAGS)

clean:
	rm -rf $(TARGET) *.o composite.txt