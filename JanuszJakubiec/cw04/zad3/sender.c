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

int sigque = 0;
int counter = 0;
int run = 1;
void usr1_handler(int sig, siginfo_t *siginfo, void *context)
{
  if(sigque == 1)
  {
    printf("%s %d sygnalow\n", "Catcher odebral", siginfo -> si_value.sival_int);
  }
  counter++;
}

void usr2_handler(int sig, siginfo_t *siginfo, void *context)
{
  run = 0;
  printf("%s %d %s\n", "Odebralem", counter, "sygnalow!");
}

void event_handler(int signum)
{
  printf("%s\n", "event has been handled!");
}

int main(int argc, char *argv[])
{
  if(argc < 4)
  {
    printf("%s\n", "Podano zbyt mala liczbe argumentow\n");
    return 0;
  }
  sigset_t newmask, block_mask;
  sigfillset(&newmask);
  sigfillset(&block_mask);
  struct sigaction act1, act2;
  sigemptyset(&act1.sa_mask);
  sigemptyset(&act2.sa_mask);
  act1.sa_sigaction = usr1_handler;
  act2.sa_sigaction = usr2_handler;
  if(strcmp(argv[3], "SIGRT") != 0)
  {
    sigdelset(&newmask, SIGUSR1);
    sigdelset(&newmask, SIGUSR2);
    sigaddset(&act1.sa_mask, SIGUSR2);
    sigaddset(&act2.sa_mask, SIGUSR1);
    act2.sa_flags = SA_SIGINFO;
    act1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act1, NULL);
    sigaction(SIGUSR2, &act2, NULL);
    sigdelset(&block_mask, SIGUSR1);
    sigdelset(&block_mask, SIGUSR2);
  }
  else
  {
    sigdelset(&newmask, SIGRTMIN+1);
    sigdelset(&newmask, SIGRTMIN+2);
    sigaddset(&act1.sa_mask,SIGRTMIN+2);
    sigaddset(&act2.sa_mask,SIGRTMIN+1);
    act2.sa_flags = SA_SIGINFO;
    act1.sa_flags = SA_SIGINFO;
    sigaction(SIGRTMIN+1, &act1, NULL);
    sigaction(SIGRTMIN+2, &act2, NULL);
    sigdelset(&block_mask, SIGRTMIN+1);
    sigdelset(&block_mask, SIGRTMIN+2);
  }
  sigprocmask(SIG_BLOCK, &block_mask, NULL);
  int pid = atoi(argv[1]);
  int n = atoi(argv[2]);
  if(strcmp(argv[3], "kill") == 0)
  {
    for(int i = 0; i<n; i++)
    {
      if(kill(pid, SIGUSR1) == -1)
      {
        printf("%s\n", "NO PROGRAM WITH SUCH PID");
        return -1;
      }
    }
    kill(pid, SIGUSR2);
    while(run == 1)
    {
      sigsuspend(&newmask);
    }
  }
  if(strcmp(argv[3], "SIGQUEUE") == 0)
  {
    sigque = 1;
    union sigval value;
    for(int i = 0; i < n; i++)
    {
      if(sigqueue(pid, SIGUSR1,value)!=0)
      {
        printf("%s\n", "NO PROGRAM WITH SUCH PID");
        return -1;
      }
    }
    sigqueue(pid, SIGUSR2,value);
    counter = 0;
    sigque = 0;
    while(run == 1)
    {
    sigsuspend(&newmask);
    }
  }
  if(strcmp(argv[3], "SIGRT") == 0)
  {
    for(int i = 0; i<n; i++)
    {
      if(kill(pid, SIGRTMIN+1)!=0)
      {
        printf("%s\n", "NO PROGRAM WITH SUCH PID");
        return -1;
      }
    }
    kill(pid,SIGRTMIN+2);
    while(run == 1)
    {
    sigsuspend(&newmask);
    }
  }
  return 0;
}
