CC = g++
CFLAGS = -Wall
LFLAGS = -lGL -lGLU -lglut -lCg -lCgGL

bump: main.cpp
	$(CC) $< -o $@ $(CFLAGS) $(LFLAGS)


