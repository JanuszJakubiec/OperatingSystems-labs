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

tests:
	make static
	touch result.txt
	echo "5 files with 1000000 lines" > result.txt
	./main ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 >> result.txt
	echo "" >> result.txt
	echo "5 files with 10000 lines" >> result.txt
	./main aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 >> result.txt
	echo "" >> result.txt
	echo "5 files with 1000 lines" >> result.txt
	./main ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 >> result.txt
	echo "" >> result.txt
	echo "50 files with 1000000 lines" >> result.txt
	./main ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 >> result.txt
	echo "" >> result.txt
	echo "50 files with 10000 lines" >> result.txt
	./main aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 >> result.txt
	echo "" >> result.txt
	echo "50 files with 1000 lines" >> result.txt
	./main ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 >> result.txt
	echo "" >> result.txt
	echo "300 files with 1000000 lines" >> result.txt
	./main ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 ai0:ai1 >> result.txt
	echo "" >> result.txt
	echo "300 files with 10000 lines" >> result.txt
	./main aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 aj0:aj1 >> result.txt
	echo "" >> result.txt
	echo "300 files with 1000 lines" >> result.txt
	./main ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 ak0:ak1 >> result.txt
	echo "" >> result.txt
