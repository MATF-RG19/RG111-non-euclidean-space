PROGRAM = main
CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lGLU -lglut -lm

$(PROGRAM): main.o input.o
	$(CC) $(CFLAGS) $(LIBS) -o $(PROGRAM) main.o input.o

main.o: main.c
	$(CC) $(CFLAGS) $(LIBS) -c main.c -o main.o

input.o: input.c
	$(CC) $(CFLAGS) -c input.c -o input.o

.PHONY: clean

clean:
	rm $(PROGRAM)
