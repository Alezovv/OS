CC = gcc
CFLAGS = -I./include
LDFLAGS = -pthread
TARGET = main

BINDIR = bin

OBJS = $(BINDIR)/main.o $(BINDIR)/Centroids.o

.PHONY: all clean

all: $(BINDIR)/$(TARGET)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/$(TARGET): $(BINDIR) $(OBJS)
	$(CC) -o $(BINDIR)/$(TARGET) $(OBJS) $(LDFLAGS)

$(BINDIR)/main.o: src/main.c include/Centroids.h
	$(CC) -c -o $(BINDIR)/main.o src/main.c $(CFLAGS)

$(BINDIR)/Centroids.o: src/Centroids.c include/Centroids.h
	$(CC) -c -o $(BINDIR)/Centroids.o src/Centroids.c $(CFLAGS)

clean:
	rm -rf $(BINDIR)