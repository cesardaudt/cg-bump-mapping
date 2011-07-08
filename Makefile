CC = g++
OBJ = vector3f.o
CFLAGS = -Wall
LFLAGS = -lGL -lGLU -lglut -lCg -lCgGL -lSDL_image

bump: main.cpp $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LFLAGS)

%.o: %.cpp %.h
	$(CC) $< -c -o $@ $(CFLAGS)

