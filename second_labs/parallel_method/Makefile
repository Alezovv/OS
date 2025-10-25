CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./src -fopenmp     
LDFLAGS = -pthread -fopenmp                            
TARGET = main
BINDIR = bin
SRCS = src/main.c src/Centroids.c
OBJS = $(SRCS:src/%.c=$(BINDIR)/%.o)

.PHONY: all clean run

all: $(BINDIR)/$(TARGET)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BINDIR)/%.o: src/%.c | $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR)

run: all
	./$(BINDIR)/$(TARGET)
