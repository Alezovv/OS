CC = gcc
CFLAGS = -I./include
LDFLAGS = -pthread
TARGET = main

BINDIR = bin

OBJS = $(BINDIR)/main.o $(BINDIR)/Custom.o $(BINDIR)/Utils.o $(BINDIR)/Validation.o

.PHONY: all clean

all: $(BINDIR)/$(TARGET)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/$(TARGET): $(BINDIR) $(OBJS)
	$(CC) -o $(BINDIR)/$(TARGET) $(OBJS) $(LDFLAGS)

$(BINDIR)/main.o: src/main.c include/Custom.h include/Utils.h include/Validation.h
	$(CC) -c -o $(BINDIR)/main.o src/main.c $(CFLAGS)

$(BINDIR)/Custom.o: src/Custom.c include/Custom.h
	$(CC) -c -o $(BINDIR)/Custom.o src/Custom.c $(CFLAGS)

$(BINDIR)/Utils.o: src/Utils.c include/Utils.h
	$(CC) -c -o $(BINDIR)/Utils.o src/Utils.c $(CFLAGS)

$(BINDIR)/Validation.o: src/Validation.c include/Validation.h
	$(CC) -c -o $(BINDIR)/Validation.o src/Validation.c $(CFLAGS)

clean:
	rm -rf $(BINDIR)