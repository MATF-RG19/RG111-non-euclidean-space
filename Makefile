PROGRAM = main
CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGL -lGLU -lglut -lm

$(PROGRAM): main.o shared.o input.o util.o light.o portal.o wall.o logic.o image.o render.o level.o
	$(CC) $(CFLAGS) -o $(PROGRAM) main.o shared.o input.o util.o light.o portal.o wall.o logic.o image.o render.o level.o  $(LIBS)

main.o: main.c shared.h util.h input.h light.h portal.h wall.h logic.h bitmaps.h render.h level.h
	$(CC) $(CFLAGS) -c main.c -o main.o

shared.o: shared.c shared.h
	$(CC) $(CFLAGS) -c shared.c -o shared.o

input.o: input.c input.h shared.h
	$(CC) $(CFLAGS) -c input.c -o input.o

light.o: light.c light.h
	$(CC) $(CFLAGS) -c light.c -o light.o

util.o: util.c util.h shared.h
	$(CC) $(CFLAGS) -c util.c -o util.o

portal.o: portal.c portal.h shared.h light.h util.h
	$(CC) $(CFLAGS) -c portal.c -o portal.o

wall.o: wall.c wall.h shared.h light.h util.h
	$(CC) $(CFLAGS) -c wall.c -o wall.o

logic.o: logic.c logic.h shared.h portal.h wall.h
	$(CC) $(CFLAGS) -c logic.c -o logic.o

image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c -o image.o

render.o: render.c render.h image.h
	$(CC) $(CFLAGS) -c render.c -o render.o

level.o: level.c level.h shared.h light.h wall.h portal.h logic.h render.h
	$(CC) $(CFLAGS) -c level.c -o level.o

.PHONY: clean

clean:
	rm $(PROGRAM) *.o
