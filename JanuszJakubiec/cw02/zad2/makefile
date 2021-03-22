CC = gcc
CFLAGS = -Wall -std=c11

system_files_functions:
	$(CC) $(CFLAGS) main_s.c -o main_s

library_files_functions:
	$(CC) $(CFLAGS) main_l.c -o main_l

clean:
	rm main_s
	rm main_l

tests:
	make system_files_functions
	touch pomiar_zad_2.txt
	echo "Funkcje systemowe:" > pomiar_zad_2.txt
	echo "real: " >> pomiar_zad_2.txt
	echo "user: " >> pomiar_zad_2.txt
	echo "system: " >> pomiar_zad_2.txt
	time ./main_s a a
	sleep 10
	make library_files_functions
	echo "Funkcje biblioteczne:" >> pomiar_zad_2.txt
	echo "real: " >> pomiar_zad_2.txt
	echo "user: " >> pomiar_zad_2.txt
	echo "system: " >> pomiar_zad_2.txt
	time ./main_l a a
	make clean
