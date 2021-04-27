CC = gcc
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) server.c -o server -lrt
	$(CC) $(CFLAGS) client.c -o client -lrt

clean:
	rm main

test:
	./main txt.txt
