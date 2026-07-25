#Variables
CC = gcc
WIN_CC = x86_64-w64-mingw32-gcc

CFLAGS = -Wall -std=c99
RAYLIB_WIN_PATH = ./raylib

all:
	$(CC) flocking.c -o fish_school $(CFLAGS) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

win:
	$(WIN_CC) flocking.c -o fish_school.exe $(CFLAGS) \
		-I$(RAYLIB_WIN_PATH)/include \
		-L$(RAYLIB_WIN_PATH)/lib \
		-lraylib -lopengl23 -lgdi32 -lwinmm -lm -mwindows

clean:
	rm -f fish_school fish_school.exe
