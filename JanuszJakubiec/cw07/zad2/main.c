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

void sig_int_handler(int sig)
{
  kill(0, SIGINT);
  sem_unlink(OVEN_BLOCKER);
  sem_unlink(OVEN_FREE);
  sem_unlink(TABLE_BLOCKER);
  sem_unlink(TABLE_FREE);
  sem_unlink(NUMBER_OF_PIZZAS);
  shm_unlink(SHARED_OVEN_NAME);
  shm_unlink(SHARED_TABLE_NAME);
  exit(0);
}

void reset_semafors_and_shared_memory()
{
  oven = (struct oven*) mmap(NULL, sizeof(struct oven), PROT_READ|PROT_WRITE, MAP_SHARED, oven_shared, 0);
  table = (struct table*) mmap(NULL, sizeof(struct table), PROT_READ|PROT_WRITE, MAP_SHARED, table_shared, 0);
  for(int i = 0; i<OVEN_SIZE; i++)
  {
    oven->table[i] = -1;
  }
  oven->pizzas_given = 0;
  oven->pizzas_taken = 0;
  for(int i = 0; i<TABLE_SIZE; i++)
  {
    table->table[i] = -1;
  }
  table->pizzas_given = 0;
  table->pizzas_taken = 0;
  munmap(table, sizeof(struct table));
  munmap(oven, sizeof(struct oven));
}

void crete_semaphors()
{
  oven_blocker = sem_open(OVEN_BLOCKER, O_CREAT | O_RDWR, 0777, 1);
  oven_free = sem_open(OVEN_FREE, O_CREAT | O_RDWR, 0777, OVEN_SIZE);
  table_blocker = sem_open(TABLE_BLOCKER, O_CREAT | O_RDWR, 0777, 1);
  table_free = sem_open(TABLE_FREE, O_CREAT | O_RDWR, 0777, TABLE_SIZE);
  table_pizzas = sem_open(NUMBER_OF_PIZZAS, O_CREAT | O_RDWR, 0777, 0);
  sem_close(oven_blocker);
  sem_close(oven_free);
  sem_close(table_blocker);
  sem_close(table_free);
  sem_close(table_pizzas);
}

int main(int argc, char *argv[])
{
  srand(time(NULL));
  if(argc < 3)
  {
    printf("%s\n", "Not enaugh arguments");
    return 0;
  }
  struct sigaction act1;
  int number_of_chefs = atoi(argv[1]);
  int number_of_deliverers = atoi(argv[2]);
  oven_shared = shm_open(SHARED_OVEN_NAME, O_CREAT | O_RDWR, 0777);
  table_shared = shm_open(SHARED_TABLE_NAME, O_CREAT | O_RDWR, 0777);
  ftruncate(oven_shared, sizeof(struct oven));
  ftruncate(table_shared, sizeof(struct table));
  crete_semaphors();
  reset_semafors_and_shared_memory();
  for(int i = 1; i < number_of_chefs+1; i++)
  {
    pid_t my_child_pid = fork();
    char num[20];
    sprintf(num, "%d", rand());
    if(my_child_pid == 0)
    {
      execl("./chef", "chef", num, NULL);
      exit(0);
    }
  }
  for(int i = 1; i < number_of_deliverers+1; i++)
  {
    pid_t my_child_pid = fork();
    char num[20];
    sprintf(num, "%d", rand());
    if(my_child_pid == 0)
    {
      execl("./deliverer", "deliverer", num, NULL);
      exit(0);
    }
  }
  sigemptyset(&act1.sa_mask);
  act1.sa_flags = 0;
  act1.sa_handler = sig_int_handler;
  sigaction(SIGINT, &act1, NULL);
  for(int i = 0; i<number_of_chefs+number_of_deliverers; i++)
  {
    wait(0);
  }
  sig_int_handler(10);
}
