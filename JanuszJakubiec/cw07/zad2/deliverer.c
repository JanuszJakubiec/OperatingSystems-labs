#include "header.h"

int table_shared;
sem_t* table_blocker;
sem_t* table_free;
sem_t* table_pizzas;
struct table* table;
char timestamp[30];

void sleep_for(int bot, int up)
{
  int n = rand()%(1000*(up-bot)) + bot*1000;
  usleep(1000*n);
}

char* print_current_time()
{
  struct timeval time_now;
  gettimeofday(&time_now, NULL);
  time_t msecs_time = (time_now.tv_sec * 1000000) + (time_now.tv_usec);
  sprintf(timestamp, "%lu", msecs_time);
  return timestamp;
}

void sig_int_handler(int sig)
{
  sem_close(table_blocker);
  sem_close(table_free);
  sem_close(table_pizzas);
  munmap(table, sizeof(struct table));
}

int get_pizza()
{
  int n = table->table[table->pizzas_taken%TABLE_SIZE];
  table->table[table->pizzas_taken%TABLE_SIZE] = -1;
  table->pizzas_taken += 1;
  return n;
}

int main(int argc, char *argv[])
{
  srand(atoi(argv[1]));
  table_blocker = sem_open(TABLE_BLOCKER, O_RDWR);
  table_free = sem_open(TABLE_FREE, O_RDWR);
  table_pizzas = sem_open(NUMBER_OF_PIZZAS, O_RDWR);
  struct sigaction act1;
  sigemptyset(&act1.sa_mask);
  act1.sa_flags = 0;
  act1.sa_handler = sig_int_handler;
  sigaction(SIGINT, &act1, NULL);
  table_shared = shm_open(SHARED_TABLE_NAME,O_RDWR, 0777);
  table = (struct table*) mmap(NULL, sizeof(struct table), PROT_READ|PROT_WRITE, MAP_SHARED, table_shared, 0);
  while(1)
  {
    sem_wait(table_blocker);
    while(sem_trywait(table_pizzas) == -1)
    {
      sem_post(table_blocker);
      sem_wait(table_blocker);
    }
    sem_post(table_free);
    int n = get_pizza();
    printf("%d %s %s%d%s%d\n", getpid(), print_current_time(), "Pobieram pizze: ", n, " Liczba pizz na stole: ", table->pizzas_given-table->pizzas_taken);
    sem_post(table_blocker);
    sleep_for(4,5);
    printf("%d %s %s%d\n", getpid(), print_current_time(), "Dostarczam pizze: ", n);
    sleep_for(4,5);
  }
}
