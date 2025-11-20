CC = gcc
CFLAGS = -Wall -Wextra -fPIC -I./include
LDFLAGS = -lm -ldl

SRCDIR = src
INCDIR = include
LIBDIR = lib
OBJDIR = obj

all: directories lib/libarea.so lib/libarea2.so static_main dynamic_main

directories:
	mkdir -p $(OBJDIR) $(LIBDIR)

# Первая реализация
$(OBJDIR)/Area.o: $(SRCDIR)/Area.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/Pi.o: $(SRCDIR)/Pi.c
	$(CC) $(CFLAGS) -c $< -o $@

lib/libarea.so: $(OBJDIR)/Area.o $(OBJDIR)/Pi.o
	$(CC) -shared -o $@ $^ -lm

# Вторая реализация
$(OBJDIR)/Area2.o: $(SRCDIR)/Area2.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/Pi2.o: $(SRCDIR)/Pi2.c
	$(CC) $(CFLAGS) -c $< -o $@

lib/libarea2.so: $(OBJDIR)/Area2.o $(OBJDIR)/Pi2.o
	$(CC) -shared -o $@ $^ -lm

# Программы
$(OBJDIR)/static_main.o: $(SRCDIR)/static_main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/dynamic_main.o: $(SRCDIR)/dynamic_main.c
	$(CC) $(CFLAGS) -c $< -o $@

static_main: $(OBJDIR)/static_main.o $(OBJDIR)/Area.o $(OBJDIR)/Pi.o
	$(CC) -o $@ $^ $(LDFLAGS)

dynamic_main: $(OBJDIR)/dynamic_main.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(OBJDIR) $(LIBDIR) static_main dynamic_main

.PHONY: all clean directories