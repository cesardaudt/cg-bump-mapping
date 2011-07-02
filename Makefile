CC = g++
CFLAGS = -Wall
LFLAGS = -lGL -lGLU -lglut -lCg -lCgGL -lSDL_image

bump: main.cpp
	$(CC) $< -o $@ $(CFLAGS) $(LFLAGS)


