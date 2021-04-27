CC = gcc
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm main

tests:
	make all
	./main test1-1
	./main test1-2
	./main test2-1
	./main test2-2
	./main test2-3
	./main test3-1
	./main test3-2
	./main test3-3
	./main test3-4
