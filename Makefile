PROGRAM = main
CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lGLU -lglut -lm

$(PROGRAM): main.o input.o util.o shared.o light.o
	$(CC) $(CFLAGS) $(LIBS) -o $(PROGRAM) main.o input.o light.o util.o shared.o

main.o: main.c
	$(CC) $(CFLAGS) $(LIBS) -c main.c -o main.o

input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c -o input.o

light.o: light.c light.h
	$(CC) $(CFLAGS) -c light.c -o light.o

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c -o util.o

shared.o: shared.c shared.h
	$(CC) $(CFLAGS) -c shared.c -o shared.o

.PHONY: clean

clean:
	rm $(PROGRAM) *.o
