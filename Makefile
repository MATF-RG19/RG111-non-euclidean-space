PROGRAM = main
CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lGLU -lglut -lm

$(PROGRAM): main.o input.o util.o
	$(CC) $(CFLAGS) $(LIBS) -o $(PROGRAM) main.o input.o util.o

main.o: main.c
	$(CC) $(CFLAGS) $(LIBS) -c main.c -o main.o

input.o: input.c
	$(CC) $(CFLAGS) -c input.c -o input.o

util.o: util.c
	$(CC) $(CFLAGS) -c util.c -o util.o

.PHONY: clean

clean:
	rm $(PROGRAM)
