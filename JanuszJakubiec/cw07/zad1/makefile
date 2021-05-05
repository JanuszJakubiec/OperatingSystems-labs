CC = gcc
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) main.c -o main
	$(CC) $(CFLAGS) deliverer.c -o deliverer
	$(CC) $(CFLAGS) chef.c -o chef

clean:
	rm main
	rm deliverer
	rm chef

test:
	./main txt.txt
