PROGRAM = main
CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lglut

$(PROGRAM):
	$(CC) $(CFLAGS) $(LIBS) -o $(PROGRAM) $(PROGRAM).c

.PHONY: clean

clean:
	rm $(PROGRAM)
