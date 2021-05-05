#include "header.h"

int* tab;
int oven_shared;
int table_shared;
int oven_semafors;
int table_semafors;

void sig_int_handler(int sig)
{
  int size = tab[0];
  for(int i = 1; i<size+1; i++)
  {
    kill(tab[i],SIGINT);
  }
  semctl(oven_semafors, 0, IPC_RMID);
  semctl(table_semafors, 0, IPC_RMID);
  shmctl(oven_shared,IPC_RMID,NULL);
  shmctl(table_shared,IPC_RMID,NULL);
  exit(0);
}

void reset_semafors_and_shared_memory()
{
  struct table* table;
  struct oven* oven;
  oven = (struct oven*) shmat(oven_shared, NULL, 0);
  table = (struct table*) shmat(table_shared, NULL, 0);
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
  union semun arg;
  arg.val = OVEN_SIZE;
  semctl(oven_semafors, OVEN_FREE_SPACE, SETVAL, arg);
  arg.val = 1;
  semctl(oven_semafors, OVEN_SHARED_AVAILABLE, SETVAL, arg);
  arg.val = TABLE_SIZE;
  semctl(table_semafors, TABLE_FREE_SPACE, SETVAL, arg);
  arg.val = 0;
  semctl(table_semafors, NUMBER_OF_PIZZAS, SETVAL, arg);
  arg.val = 1;
  semctl(table_semafors, TABLE_SHARED_AVAILABLE, SETVAL, arg);
  shmdt(oven);
  shmdt(table);
}

int main(int argc, char *argv[])
{
  if(argc < 3)
  {
    printf("%s\n", "Not enaugh arguments");
    return 0;
  }
  struct sigaction act1;
  int number_of_chefs = atoi(argv[1]);
  int number_of_deliverers = atoi(argv[2]);
  tab = calloc(number_of_chefs + number_of_deliverers+1, sizeof(int));
  tab[0] = number_of_chefs + number_of_deliverers;
  oven_shared = shmget(ftok(HOME, 'o'),sizeof(struct oven), IPC_CREAT | 0777);
  table_shared = shmget(ftok(HOME, 't'),sizeof(struct table), IPC_CREAT | 0777);
  oven_semafors = semget(ftok(HOME, 'o'), OVEN_SEMAFORS, IPC_CREAT | 0777); // 0 -> free space in oven, 1 -> flag if sb is using oven_table
  table_semafors = semget(ftok(HOME, 't'), TABLE_SEMAFORS, IPC_CREAT | 0777); // 0 -> free space on the table 1 -> number of pizzas on the table 2 -> flag if sb is using table_table
  reset_semafors_and_shared_memory();
  for(int i = 1; i < number_of_chefs+1; i++)
  {
    pid_t my_child_pid = fork();
    if(my_child_pid == 0)
    {
      execl("./chef", "chef", NULL);
      exit(0);
    }
    sleep_for_less(100, 200);
    tab[i] = my_child_pid;
  }
  for(int i = 1; i < number_of_deliverers+1; i++)
  {
    pid_t my_child_pid = fork();
    if(my_child_pid == 0)
    {
      execl("./deliverer", "deliverer", NULL);
      exit(0);
    }
    sleep_for_less(1, 2);
    tab[i+number_of_chefs] = my_child_pid;
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
