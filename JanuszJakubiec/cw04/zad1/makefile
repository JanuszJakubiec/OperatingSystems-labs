CC = gcc
CFLAGS = -Wall

fork:
	$(CC) $(CFLAGS) main_f.c -o main

exec:
	$(CC) $(CFLAGS) main_e.c -o main
	$(CC) $(CFLAGS) child.c -o child

clean:
	rm main

tests:
	make fork
	./main ignore
	./main handler
	./main mask
	./main pending
	make clean
	make exec
	./main ignore
	./main mask
	./main pending
	make clean
