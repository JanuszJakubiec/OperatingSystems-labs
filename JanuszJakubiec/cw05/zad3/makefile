CC = gcc
cflags = -Wall

all:
	$(CC) $(cflags) generate.c -o generate
	$(CC) $(cflags) main.c -o main
	$(CC) $(cflags) producent.c -o producent
	$(CC) $(cflags) customer.c -o customer
	touch result.txt
	./main 5 5 1 100
	cp result.txt result
	cp proper.txt proper
	make clean

clean:
	rm *.txt
	RM zad3pipe

tests:
	$(CC) $(cflags) generate.c -o generate
	$(CC) $(cflags) main.c -o main
	$(CC) $(cflags) producent.c -o producent
	$(CC) $(cflags) customer.c -o customer
	touch result.txt
	./main 5000 5 1 8000
	cp result.txt result
	cp proper.txt proper
	make clean
