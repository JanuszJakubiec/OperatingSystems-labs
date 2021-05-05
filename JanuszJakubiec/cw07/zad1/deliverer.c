#include "header.h"

int table_shared;
int table_semafors;
struct table* table;

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
  srand(time(NULL));
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
    printf("%d %lu %s%d%s%d\n", getpid(), (unsigned long)time(NULL), "Pobieram pizze: ", n, " Liczba pizz na stole: ", table->pizzas_given-table->pizzas_taken);
    send_instructions(table_semafors, unlock_table, 3);
    sleep_for(4,5);
    printf("%d %lu %s%d\n", getpid(), (unsigned long)time(NULL), "Dostarczam pizze: ", n);
    sleep_for(4,5);
  }
}
