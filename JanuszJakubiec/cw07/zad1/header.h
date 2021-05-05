#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <bits/sigaction.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define OVEN_SIZE 5
#define TABLE_SIZE 5
#define HOME getenv("HOME")
#define TABLE_SEMAFORS 3
#define OVEN_SEMAFORS 2

#define OVEN_FREE_SPACE 0
#define OVEN_SHARED_AVAILABLE 1

#define TABLE_FREE_SPACE 0
#define NUMBER_OF_PIZZAS 1
#define TABLE_SHARED_AVAILABLE 2

struct oven{
  int table[OVEN_SIZE];
  int pizzas_given;
  int pizzas_taken;
};

struct table{
  int table[TABLE_SIZE];
  int pizzas_given;
  int pizzas_taken;
};

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *__buf;
};

void sleep_for(int bot, int up)
{
  int n = rand()%(1000*(up-bot)) + bot*1000;
  usleep(1000*n);
}

void sleep_for_less(int bot, int up)
{
  int n = rand()%(1000*(up-bot)) + bot*1000;
  usleep(10*n);
}
struct sembuf* create_instructions(int* tab, int size)
{
  int k = size/3;
  struct sembuf* instructions = calloc(k, sizeof(struct sembuf));
  for(int i = 0; i<size; i += 3)
  {
    int k = i/3;
    instructions[k].sem_num = tab[i];
    instructions[k].sem_op = tab[i+1];
    instructions[k].sem_flg = tab[i+2];
  }
  return instructions;
}

void send_instructions(int identifier, int* tab, int size)
{
  struct sembuf* instructions = create_instructions(tab, size);
  int k = size/3;
  semop(identifier, instructions,k);
  free(instructions);
}
