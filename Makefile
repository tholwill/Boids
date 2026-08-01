#Variables
CC = gcc
WIN_CC = x86_64-w64-mingw32-gcc

CFLAGS = -Wall -std=c99
RAYLIB_WIN_PATH = ./raylib/src

all:
	$(CC) flocking.c -o boids $(CFLAGS) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

win:
	$(WIN_CC) flocking.c -o boids.exe $(CFLAGS) \
		-I$(RAYLIB_WIN_PATH) \
		-L$(RAYLIB_WIN_PATH) \
		-lraylib -lopengl32 -lgdi32 -lwinmm -lm -mwindows

clean:
	rm -f boids boids.exe
