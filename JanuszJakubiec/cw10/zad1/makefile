CC = gcc
CFLAGS = -Wall
FILE_NAME = totem.ascii.pgm

all:
	$(CC) $(CFLAGS) server.c -o server -lpthread
	$(CC) $(CFLAGS) client.c -o client -lpthread

clean:
	rm main

test:
	make all
	./main
