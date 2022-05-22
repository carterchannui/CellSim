CC = gcc
CFLAGS = -Wall -g

# Build an executable named game from game.c
all : cellsim.c
	$(CC) cellsim.c `pkg-config --libs --cflags raylib` -o cellsim

clean :
	rm cellsim
