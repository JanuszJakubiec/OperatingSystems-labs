CC = gcc
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) main.c -o main -lpthread -lrt
	$(CC) $(CFLAGS) deliverer.c -o deliverer -lpthread -lrt
	$(CC) $(CFLAGS) chef.c -o chef -lpthread -lrt

clean:
	rm main
	rm deliverer
	rm chef

test:
	make all
	./main 10 10
