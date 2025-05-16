CC = gcc
CFLAGS = -Wall -I. $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2)

all: cinoop.exe

cinoop.exe: main.o cpu.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o *.exe
