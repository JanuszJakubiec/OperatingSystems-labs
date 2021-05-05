#include "header.h"

int oven_shared;
int table_shared;
int oven_semafors;
int table_semafors;
struct oven* oven;
struct table* table;

void sig_int_handler(int sig)
{
  shmdt(oven);
  shmdt(table);
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
  srand(time(NULL));
  int unlock_oven[] = {OVEN_SHARED_AVAILABLE, 1, 0};
  int insert_pizza_to_oven[] = {OVEN_FREE_SPACE, -1, 0, OVEN_SHARED_AVAILABLE, -1, 0};
  int take_pizza_out_of_oven[] = {OVEN_SHARED_AVAILABLE, -1, 0, OVEN_FREE_SPACE, 1, 0};
  int put_pizza_on_the_table[] = {NUMBER_OF_PIZZAS, 1, 0, TABLE_SHARED_AVAILABLE, -1, 0, TABLE_FREE_SPACE, -1, 0};
  int unlock_table[] = {TABLE_SHARED_AVAILABLE, 1, 0};
  struct sigaction act1;
  sigemptyset(&act1.sa_mask);
  act1.sa_flags = 0;
  act1.sa_handler = sig_int_handler;
  sigaction(SIGINT, &act1, NULL);
  oven_shared = shmget(ftok(HOME, 'o'),sizeof(struct oven), 0);
  table_shared = shmget(ftok(HOME, 't'),sizeof(struct table),  0);
  oven_semafors = semget(ftok(HOME, 'o'), 0, 0); // 0 -> free space in oven, 1 -> flag if sb is using oven_table
  table_semafors = semget(ftok(HOME, 't'), 0, 0); // 0 -> free space on the table 1 -> number of pizzas on the table 2 -> flag if sb is using table_table
  oven = (struct oven*) shmat(oven_shared, NULL, 0);
  table = (struct table*) shmat(table_shared, NULL, 0);
  while(1)
  {
    int n = rand()%9;
    printf("%d %lu %s%d\n", getpid(), (unsigned long)time(NULL), "Przygotowuje pizze: ", n);
    sleep_for(1,2);
    send_instructions(oven_semafors, insert_pizza_to_oven, 6);
    insert_pizza(n);
    printf("%d %lu %s%d%s%d\n", getpid(), (unsigned long)time(NULL), "Dodalem pizze: ", n, " Liczba pizz w piecu: ", oven->pizzas_given-oven->pizzas_taken);
    send_instructions(oven_semafors, unlock_oven, 3);
    sleep_for(4,5);
    send_instructions(oven_semafors, take_pizza_out_of_oven, 6);
    n = take_pizza();
    printf("%d %lu %s%d%s%d\n", getpid(), (unsigned long)time(NULL), "Pobieram pizze: ", n, " Liczba pizz w piecu: ", oven->pizzas_given-oven->pizzas_taken);
    send_instructions(oven_semafors, unlock_oven, 3);
    send_instructions(table_semafors, put_pizza_on_the_table, 9);
    insert_pizza_to_table_table(n);
    printf("%d %lu %s%d%s%d\n", getpid(), (unsigned long)time(NULL), "Klade pizze na stol: ", n, " Liczba pizz na stole: ", table->pizzas_given-table->pizzas_taken);
    send_instructions(table_semafors, unlock_table, 3);
  }

}
