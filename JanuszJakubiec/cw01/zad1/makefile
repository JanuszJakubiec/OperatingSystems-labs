CC = gcc
CFLAGS = -Wall -std=c11

static: main_static

main_static: main.o static_file_mergerer.o
	$(CC) $(CFLAGS) main.o static_file_mergerer.o -o main

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

static_file_mergerer.o: file_mergerer.c
	$(CC) $(CFLAGS) -o static_file_mergerer.o -c file_mergerer.c

shared: main_shared

shared_file_mergerer.o: file_mergerer.c
	$(CC) $(CFLAGS) -fPIC -c file_mergerer.c -o shared_file_mergerer.o

libshared_file_mergerer.so: shared_file_mergerer.o
	$(CC) $(CFLAGS) -shared -o libshared_file_mergerer.so shared_file_mergerer.o

main_shared: main.o libshared_file_mergerer.so
	$(CC) main.c -lshared_file_mergerer -L ./ -o main -Wl,-rpath=.

clean:
	rm *o rm main
