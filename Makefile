PROGRAM = main
CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lGLU -lglut -lm

$(PROGRAM): main.c
	$(CC) $(CFLAGS) $(LIBS) -o $(PROGRAM) $(PROGRAM).c

.PHONY: clean

clean:
	rm $(PROGRAM)
