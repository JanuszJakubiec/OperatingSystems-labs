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
#include <sys/time.h>
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