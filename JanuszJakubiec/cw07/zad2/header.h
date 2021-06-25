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
#include <semaphore.h>
#include <sys/mman.h>

#define OVEN_SIZE 5
#define TABLE_SIZE 5
#define HOME getenv("HOME")

#define SHARED_OVEN_NAME "/shared_oven"
#define SHARED_TABLE_NAME "/shared_table"

#define TABLE_BLOCKER "/table_blocker"
#define TABLE_FREE "/table_free"
#define OVEN_FREE "/oven_free"
#define NUMBER_OF_PIZZAS "/number_of_pizzas"
#define OVEN_BLOCKER "/oven_blocker"

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
