#include "header.h"

int oven_shared;
int table_shared;
sem_t* oven_blocker;
sem_t* oven_free;
sem_t* table_blocker;
sem_t* table_free;
sem_t* table_pizzas;
struct oven* oven;
struct table* table;
char timestamp[30];

void sig_int_handler(int sig)
{
  sem_close(oven_blocker);
  sem_close(oven_free);
  sem_close(table_blocker);
  sem_close(table_free);
  sem_close(table_pizzas);
  munmap(table, sizeof(struct table));
  munmap(oven, sizeof(struct oven));
}

void sleep_for(int bot, int up)
{
  int n = rand()%(1000*(up-bot)) + bot*1000;
  usleep(1000*n);
}

void sleep_for_short(int bot, int up)
{
  int n = rand()%(1000*(up-bot)) + bot*1000;
  usleep(10*n);
}

char* print_current_time()
{
  struct timeval time_now;
  gettimeofday(&time_now, NULL);
  time_t msecs_time = (time_now.tv_sec * 1000000) + (time_now.tv_usec);
  sprintf(timestamp, "%lu", msecs_time);
  return timestamp;
}

int take_pizza()
{
  int n = oven->table[oven->pizzas_taken%OVEN_SIZE];
  oven->table[oven->pizzas_taken%OVEN_SIZE] = -1;
  oven->pizzas_taken += 1;
  return n;
}

void insert_pizza(int n)
{
  oven->table[(oven->pizzas_given)%OVEN_SIZE] = n;
  oven->pizzas_given += 1;
}

void insert_pizza_to_table_table(int n)
{
  table->table[(table->pizzas_given)%TABLE_SIZE] = n;
  table->pizzas_given += 1;
}

int main(int argc, char *argv[])
{
  oven_blocker = sem_open(OVEN_BLOCKER, O_RDWR);
  oven_free = sem_open(OVEN_FREE, O_RDWR);
  table_blocker = sem_open(TABLE_BLOCKER, O_RDWR);
  table_free = sem_open(TABLE_FREE, O_RDWR);
  table_pizzas = sem_open(NUMBER_OF_PIZZAS, O_RDWR);
  srand(atoi(argv[1]));
  struct sigaction act1;
  sigemptyset(&act1.sa_mask);
  act1.sa_flags = 0;
  act1.sa_handler = sig_int_handler;
  sigaction(SIGINT, &act1, NULL);
  oven_shared = shm_open(SHARED_OVEN_NAME, O_RDWR, 0777);
  table_shared = shm_open(SHARED_TABLE_NAME,O_RDWR, 0777);
  oven = (struct oven*) mmap(NULL, sizeof(struct oven), PROT_READ|PROT_WRITE, MAP_SHARED, oven_shared, 0);
  table = (struct table*) mmap(NULL, sizeof(struct table), PROT_READ|PROT_WRITE, MAP_SHARED, table_shared, 0);
  while(1)
  {
    int n = rand()%9;
    printf("%d %s %s%d\n", getpid(), print_current_time(), "Przygotowuje pizze: ", n);
    sleep_for(1,2);

    sem_wait(oven_blocker);
    while(sem_trywait(oven_free) == -1)
    {
      sem_post(oven_blocker);
      sem_wait(oven_blocker);
    }
    insert_pizza(n);
    printf("%d %s %s%d%s%d\n", getpid(), print_current_time(), "Dodalem pizze: ", n, " Liczba pizz w piecu: ", oven->pizzas_given-oven->pizzas_taken);
    sem_post(oven_blocker);

    sleep_for(4,5);

    sem_wait(oven_blocker);
    sem_post(oven_free);
    n = take_pizza();
    printf("%d %s %s%d%s%d\n", getpid(), print_current_time(), "Pobieram pizze: ", n, " Liczba pizz w piecu: ", oven->pizzas_given-oven->pizzas_taken);
    sem_post(oven_blocker);

    sem_wait(table_blocker);
    while(sem_trywait(table_free) == -1)
    {
      sem_post(table_blocker);
      sem_wait(table_blocker);
    }
    sem_post(table_pizzas);
    insert_pizza_to_table_table(n);
    printf("%d %s %s%d%s%d\n", getpid(), print_current_time(), "Klade pizze na stol: ", n, " Liczba pizz na stole: ", table->pizzas_given-table->pizzas_taken);
    sem_post(table_blocker);
  }
}
