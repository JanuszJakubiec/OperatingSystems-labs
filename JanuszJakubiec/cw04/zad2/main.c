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

void test1_handler(int sig, siginfo_t *siginfo, void *context)
{
  printf("%s %d\n", "Otrzymany sygnal ma numer:", siginfo->si_signo);
  printf("%s %d\n", "Otrzymany sygnal zostal wyslany przez program o PID:", siginfo->si_pid);
}

void test2_handler(int sig)
{
  printf("%s\n", "Po pierwszym otrzymaniu sygnalu uruchamia sie funkcja");
}

int i = 0;
void test3_handler(int sig)
{
  int j = i;
  printf("%s %d\n", "Zaczynam obsluge sygnalu", j);
  if(i < 4)
  {
    i++;
    raise(SIGUSR1);
  }
  printf("%s %d\n", "Koncze obsluge sygnalu", j);
}

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("%s\n", "NOT ENAUGH ARUGMENTS!");
    return 0;
  }
  struct sigaction act;
  sigfillset(&act.sa_mask);
  if(strcmp(argv[1], "test1-1") == 0)
  {
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    printf("%s\n", "Flaga SA_SIGINFO nic nie zmiena gdy nie ma handlera");
    raise(SIGUSR1);
  }
  if(strcmp(argv[1], "test1-2") == 0)
  {
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = test1_handler;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);
    printf("%s\n", "Flaga SA_SIGINFO pozawala uzyskac szereg informacji z sygnalu. M. innymi PID procesu, ktory go wyslal, numer sygnalu ktory zotal wyslany itd.");
    printf("%s\n", "Aby poprawnie zastosowac flage SA_SIGINFO, nalezy przypisac handler do sa_sigaction, a nie do sa_handler." );
  }
  if(strcmp(argv[1], "test2-1") == 0)
  {
    act.sa_flags = SA_RESETHAND;
    act.sa_handler = test2_handler;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);
    printf("%s\n", "Drugie wywolanie funkcji skutuje wykonaniem sie domyslnej akcji systemowej.");
    raise(SIGUSR1);
  }
  if(strcmp(argv[1], "test2-2") == 0)
  {
    act.sa_flags = SA_RESETHAND;
    act.sa_handler = SIG_IGN;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);
    raise(SIGUSR1);
    printf("%s\n", "Sygnal zostal zignorowany w kazdym przypadku, nawet przy drugim wywolaniu,");
  }
  if(strcmp(argv[1], "test2-3") == 0)
  {
    act.sa_flags = SA_RESETHAND;
    sigaction(SIGUSR1, &act, NULL);
    printf("%s\n", "Kiedy nie podam handlera, flaga nic nie zmienia. Zostaje wykonana domyslna operacja co spowoduje zakonczenie programu.");
    raise(SIGUSR1);
    printf("%s\n", "To sie nie powinno wywolac");
    raise(SIGUSR1);
  }
  if(strcmp(argv[1], "test2-4") == 0)
  {
    act.sa_flags = SA_RESETHAND;
    act.sa_handler = test2_handler;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    raise(SIGUSR1);
    raise(SIGUSR2);
    printf("%s\n", "Pierwsze odebranie sygnalu resetuje ustawienia tylko dla odebranego sygnalu.");
  }
  if(strcmp(argv[1], "test3-1") == 0)
  {
    sigset_t wait_mask;
    sigemptyset(&wait_mask);
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = test3_handler;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);
    printf("%s\n", "Bez flagi SA_NODEFER, sygnaly sygnal wywolany w funcji test3_handler jest odczytywany po wyjsciu z handlera");
  }
  if(strcmp(argv[1], "test3-2") == 0)
  {
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NODEFER;
    act.sa_handler = test3_handler;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);
    printf("%s\n", "Jezeli korzystam z flagi SA_NODEFER, sygnal jest obslugiwany wewnatrz handlera");
  }
  if(strcmp(argv[1], "test3-3") == 0)
  {
    act.sa_flags = SA_NODEFER;
    act.sa_handler = test3_handler;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);
    printf("%s\n", "Po dodaniu sygnalu do sa_mask, flaga SA_NODEFER nie dziala dla tego sygnalu");
  }
  return 0;
}
