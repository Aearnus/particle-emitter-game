CC=g++
INPUT_FILES=main.cpp
LIBS=-lsfml-graphics -lsfml-window -lsfml-system
ARGS=-g -std=c++11

all:
	$(CC) $(INPUT_FILES) -o particle.out $(ARGS) $(LIBS)
