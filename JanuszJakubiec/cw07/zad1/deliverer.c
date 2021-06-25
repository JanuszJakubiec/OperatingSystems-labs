#include "header.h"

int table_shared;
int table_semafors;
struct table* table;

char timestamp[30];

void sleep_for(int bot, int up)
{
  int n = rand()%(1000*(up-bot)) + bot*1000;
  usleep(1000*n);
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
  shmdt(table);
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
  struct sigaction act1;
  sigemptyset(&act1.sa_mask);
  act1.sa_flags = 0;
  act1.sa_handler = sig_int_handler;
  sigaction(SIGINT, &act1, NULL);
  table_shared = shmget(ftok(HOME, 't'),sizeof(struct table),  0);
  table_semafors = semget(ftok(HOME, 't'), 0, 0777); // 0 -> free space on the table 1 -> number of pizzas on the table 2 -> flag if sb is using table_table
  table = (struct table*) shmat(table_shared, NULL, 0);
  while(1)
  {
    int take_pizza_from_table[] = {NUMBER_OF_PIZZAS, -1, 0, TABLE_SHARED_AVAILABLE, -1, 0, TABLE_FREE_SPACE, 1, 0};
    int unlock_table[] = {TABLE_SHARED_AVAILABLE, 1, 0};
    send_instructions(table_semafors, take_pizza_from_table, 9);
    int n = get_pizza();
    printf("%d %s %s%d%s%d\n", getpid(), print_current_time(), "Pobieram pizze: ", n, " Liczba pizz na stole: ", table->pizzas_given-table->pizzas_taken);
    send_instructions(table_semafors, unlock_table, 3);
    sleep_for(4,5);
    printf("%d %s %s%d\n", getpid(), print_current_time(), "Dostarczam pizze: ", n);
    sleep_for(4,5);
  }
}
