#include "header.h"
int waiting_elves;
int waiting_reindeers;
int trigger_reindeers;
int trigger_elves;
int someone_waked_santa_up;
int waking_reindeer;
int who_waked_santa;
int elves_queue[SANTA_ELVES_TRIGGER];

pthread_mutex_t mutex;
pthread_mutex_t active_elves_mutex;
pthread_mutex_t active_reindeers_mutex;
pthread_mutex_t waking_mutex;
pthread_cond_t active_elves_cond;
pthread_cond_t santa_cond;
pthread_cond_t reindeer_cond;
pthread_cond_t elves_cond;
pthread_cond_t waking_cond;

void sleep_for(int bot, int up)
{
  int n = rand()%(1000*(up-bot)) + bot*1000;
  usleep(1000*n);
}

void* santa(void* arg)
{
  while(1)
  {
    pthread_mutex_lock(&waking_mutex);
    while(someone_waked_santa_up == 0)
    {
      pthread_cond_wait(&santa_cond, &waking_mutex);
    }
    pthread_mutex_unlock(&waking_mutex);

    if(who_waked_santa == 0)
    {
      pthread_mutex_lock(&active_elves_mutex);
      printf("%s\n", "Mikolaj: budze sie");
      trigger_elves = 1;
      pthread_cond_broadcast(&active_elves_cond);
      pthread_mutex_unlock(&active_elves_mutex);

      printf("%s %d, %d, %d\n", "Mikolaj: rozwiazuje problemy elfow: ", elves_queue[0], elves_queue[1], elves_queue[2]);
      sleep_for(SANTA_PROBLEM_SOLVING_MIN, SANTA_PROBLEM_SOLVING_MAX);

      pthread_mutex_lock(&active_elves_mutex);
      trigger_elves = 0;
      pthread_cond_broadcast(&active_elves_cond);
      while(waiting_elves != 0)
      {
        pthread_cond_wait(&santa_cond, &active_elves_mutex);
      }
      pthread_mutex_unlock(&active_elves_mutex);

      pthread_cond_broadcast(&elves_cond);

      pthread_mutex_lock(&waking_mutex);
      someone_waked_santa_up = 0;
      printf("%s\n", "Mikolaj: zasypiam");
      pthread_cond_broadcast(&waking_cond);
      pthread_mutex_unlock(&waking_mutex);

    }


    else
    {
      pthread_mutex_lock(&active_reindeers_mutex);
      printf("%s\n", "Mikolaj: budze sie");
      trigger_reindeers = 1;
      pthread_cond_broadcast(&reindeer_cond);
      pthread_mutex_unlock(&active_reindeers_mutex);

      printf("%s\n", "Mikolaj: Dostarczam zabawki grzecznym dzieciom i studentom ");
      sleep_for(GIFTS_DELIVERY_MIN, GIFTS_DELIVERY_MAX);

      pthread_mutex_lock(&active_reindeers_mutex);
      trigger_reindeers = 0;
      pthread_cond_broadcast(&reindeer_cond);
      while(waiting_reindeers != 0)
      {
        pthread_cond_wait(&santa_cond, &active_reindeers_mutex);
      }
      pthread_mutex_unlock(&active_reindeers_mutex);

      pthread_cond_broadcast(&reindeer_cond);

      pthread_mutex_unlock(&active_elves_mutex);
      pthread_mutex_lock(&waking_mutex);
      someone_waked_santa_up = 0;
      printf("%s\n", "Mikolaj: zasypiam");
      pthread_cond_broadcast(&waking_cond);
      pthread_mutex_unlock(&waking_mutex);
    }
  }
}

void* reindeer(void* arg)
{
  int id;
  id = *(int*)arg;
  while(1)
  {
    sleep_for(REINDEER_HOLIDAY_MIN, REINDEER_HOLIDAY_MAX);

    pthread_mutex_lock(&mutex);
    waiting_reindeers++;
    if(waiting_reindeers == SANTA_REINDEER_TRIGGER)
      waking_reindeer = id;
    printf("%s%d%s%d\n", "Renifer: Czeka ", waiting_reindeers, " reniferow na Mikolaja ", id);
    pthread_cond_broadcast(&santa_cond);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&active_reindeers_mutex);
    if(waiting_reindeers == SANTA_REINDEER_TRIGGER && waking_reindeer == id)
    {
      pthread_mutex_lock(&waking_mutex);
      while(someone_waked_santa_up == 1)
      {
        pthread_cond_wait(&waking_cond, &waking_mutex);
      }
      who_waked_santa = 1;
      someone_waked_santa_up = 1;
      printf("%s%d\n", "Renifer: Wybudzam Mikolaja ", id);
      pthread_cond_broadcast(&santa_cond);
      pthread_mutex_unlock(&waking_mutex);
    }
    while(trigger_reindeers == 0)
    {
      pthread_cond_wait(&reindeer_cond, &active_reindeers_mutex);
    }

    printf("%s%d\n", "Renifer: Dostarczam zabawki grzecznym dzieciom i studentom ", id);

    while(trigger_reindeers == 1)
    {
      pthread_cond_wait(&reindeer_cond, &active_reindeers_mutex);
    }

    waiting_reindeers--;
    pthread_cond_signal(&santa_cond);
    pthread_mutex_unlock(&active_reindeers_mutex);
  }
}

void* elf(void* arg)
{
  int counter = 0;
  int id;
  id = *(int*)arg;
  while(1)
  {
    counter = 0;
    sleep_for(ELVES_WORK_MIN, ELVES_WORK_MAX);
    pthread_mutex_lock(&mutex);

    while(waiting_elves >= SANTA_ELVES_TRIGGER)
    {
      if(counter == 0)
        printf("%s%d\n", "Elf: Czekam na powrot elfow ", id);
      counter++;
      pthread_cond_wait(&elves_cond, &mutex);
    }

    elves_queue[waiting_elves] = id;
    waiting_elves++;
    printf("%s%d%s%d\n", "Elf: Czeka ", waiting_elves, " elfow na mikolaja ", id);
    pthread_cond_broadcast(&santa_cond);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&active_elves_mutex);
    if(waiting_elves >= SANTA_ELVES_TRIGGER && elves_queue[SANTA_ELVES_TRIGGER-1] == id)
    {
      pthread_mutex_lock(&waking_mutex);
      while(someone_waked_santa_up == 1)
      {
        pthread_cond_wait(&waking_cond, &waking_mutex);
      }
      who_waked_santa = 0;
      someone_waked_santa_up = 1;
      printf("%s%d\n", "Elf: Wybudzam Mikolaja ", id);
      pthread_cond_broadcast(&santa_cond);
      pthread_mutex_unlock(&waking_mutex);
    }
    while(trigger_elves == 0)
    {
      pthread_cond_wait(&active_elves_cond, &active_elves_mutex);
    }

    printf("%s %d\n","Elf: Mikolaj rozwiazuje problem", id);

    while(trigger_elves == 1)
    {
      pthread_cond_wait(&active_elves_cond, &active_elves_mutex);
    }

    waiting_elves--;
    pthread_cond_signal(&santa_cond);
    pthread_mutex_unlock(&active_elves_mutex);
  }
}

int main(int argc, char *argv[])
{
  int elves_ids[ELVES_N];
  int reindeers_ids[REINDEER_N];
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&active_elves_mutex, NULL);
  pthread_mutex_init(&active_reindeers_mutex, NULL);
  pthread_mutex_init(&waking_mutex, NULL);
  pthread_cond_init(&active_elves_cond, NULL);
  pthread_cond_init(&santa_cond, NULL);
  pthread_cond_init(&elves_cond, NULL);
  pthread_cond_init(&reindeer_cond, NULL);
  waiting_elves = 0;
  waiting_reindeers = 0;
  trigger_elves = 0;
  trigger_reindeers = 0;
  someone_waked_santa_up = 0;
  srand(time(NULL));
  pthread_t* threads_tab = calloc(ELVES_N + REINDEER_N + 1, sizeof(pthread_t));
  pthread_create(&threads_tab[0], NULL, santa, NULL);

  for(int i = 1; i < 1+ELVES_N; i++)
  {
    elves_ids[i-1] = i-1;
    pthread_create(&threads_tab[i], NULL, elf, (void* )&elves_ids[i-1]);
  }

  for(int i = ELVES_N + 1; i < 1+ELVES_N + REINDEER_N; i++)
  {
    reindeers_ids[i-ELVES_N - 1] = i-1;
    pthread_create(&threads_tab[i], NULL, reindeer, (void* )&reindeers_ids[i-ELVES_N - 1]);
  }

  sigset_t newmask;
  sigfillset(&newmask);
  sigdelset(&newmask, SIGINT);
  sigsuspend(&newmask);
  free(threads_tab);
  return 0;
}
