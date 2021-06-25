CC = gcc
CFLAGS = -Wall
FILE_NAME = totem.ascii.pgm

all:
	$(CC) $(CFLAGS) main.c -o main -lpthread

clean:
	rm main

test:
	touch Times.txt
	make all
	echo "Numbers" > Times.txt
	echo "---------------------" >> Times.txt
	echo "Numbers 1:" >> Times.txt
	./main 1 numbers $(FILE_NAME) result.ascii.pgm >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Numbers 2:" >> Times.txt
	./main 2 numbers $(FILE_NAME) result.ascii.pgm >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Numbers 4:" >> Times.txt
	./main 4 numbers $(FILE_NAME) result.ascii.pgm >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Numbers 8:" >> Times.txt
	./main 8 numbers $(FILE_NAME) result.ascii.pgm >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Numbers 16:" >> Times.txt
	./main 16 numbers $(FILE_NAME) result.ascii.pgm >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Block:" >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Block 1:" >> Times.txt
	./main 1 block $(FILE_NAME) result.ascii.pgm >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Block 2:" >> Times.txt
	./main 2 block $(FILE_NAME) result.ascii.pgm >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Block 4:" >> Times.txt
	./main 4 block $(FILE_NAME) result.ascii.pgm >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Block 5:" >> Times.txt
	./main 5 block $(FILE_NAME) result.ascii.pgm >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Block 8:" >> Times.txt
	./main 8 block $(FILE_NAME) result.ascii.pgm >> Times.txt
	echo "---------------------" >> Times.txt
	echo "Block 16:" >> Times.txt
	./main 16 block $(FILE_NAME) result.ascii.pgm >> Times.txt
