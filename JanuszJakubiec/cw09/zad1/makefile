CC = gcc
CFLAGS = -Wall
FILE_NAME = totem.ascii.pgm

all:
	$(CC) $(CFLAGS) main.c -o main -lpthread

clean:
	rm main

test:
	make all
	./main
