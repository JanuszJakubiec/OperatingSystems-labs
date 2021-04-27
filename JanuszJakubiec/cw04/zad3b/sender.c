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
int n = 0;
int pid = 0;
int max = 0;

void com_handler(int sig, siginfo_t *siginfo, void *context)
{
  if(sigque == 1)
  {
    max = siginfo -> si_value.sival_int;
  }
}

void usr1_handler(int sig, siginfo_t *siginfo, void *context)
{
  counter++;
}

void usr2_handler(int sig, siginfo_t *siginfo, void *context)
{
  run = 0;
  printf("%s %d %s\n", "Odebralem", counter, "sygnalow!");
}

int main(int argc, char *argv[])
{
  if(argc < 3)
  {
    printf("%s\n", "Podano zbyt mala liczbe argumentow\n");
    return 0;
  }
  sigset_t newmask,block;
  sigfillset(&newmask);
  sigfillset(&block);
  sigprocmask(SIG_BLOCK, &block, NULL);
  struct sigaction act1, act2;
  sigemptyset(&act1.sa_mask);
  sigemptyset(&act2.sa_mask);
  act2.sa_sigaction = usr2_handler;
  if(strcmp(argv[3], "SIGRT") != 0)
  {
    act1.sa_sigaction = com_handler;
    sigdelset(&newmask, SIGUSR1);
    sigdelset(&newmask, SIGUSR2);
    sigaddset(&act1.sa_mask, SIGUSR2);
    sigaddset(&act2.sa_mask, SIGUSR1);
    act2.sa_flags = SA_SIGINFO;
    act1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act1, NULL);
    sigaction(SIGUSR2, &act2, NULL);
  }
  else
  {
    struct sigaction act3;
    sigdelset(&newmask, SIGRTMIN+1);
    sigdelset(&newmask, SIGRTMIN+2);
    sigdelset(&newmask, SIGUSR1);
    sigaddset(&act1.sa_mask,SIGRTMIN+2);
    sigaddset(&act2.sa_mask,SIGRTMIN+1);
    act2.sa_flags = SA_SIGINFO;
    act1.sa_flags = SA_SIGINFO;
    act3.sa_flags = SA_SIGINFO;
    act1.sa_sigaction = usr1_handler;
    act3.sa_sigaction = com_handler;
    sigaction(SIGRTMIN+1, &act1, NULL);
    sigaction(SIGRTMIN+2, &act2, NULL);
    sigaction(SIGUSR1, &act3, NULL);
  }
  pid = atoi(argv[1]);
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
      sigsuspend(&newmask);
    }
    kill(pid, SIGUSR2);
    act1.sa_sigaction = usr1_handler;
    sigaction(SIGUSR1, &act1, NULL);
    while(run == 1)
    {
      sigsuspend(&newmask);
      kill(pid, SIGUSR1);
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
      sigsuspend(&newmask);
    }
    printf("%s %d sygnalow\n", "Sender odebral", max);
    act1.sa_sigaction = usr1_handler;
    sigaction(SIGUSR1, &act1, NULL);
    sigqueue(pid, SIGUSR2,value);
    counter = 0;
    sigque = 0;
    while(run == 1)
    {
      sigsuspend(&newmask);
      sigqueue(pid, SIGUSR1, value);
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
      sigsuspend(&newmask);
    }
    kill(pid,SIGRTMIN+2);
    while(run == 1)
    {
      sigsuspend(&newmask);
      kill(pid, SIGUSR1);
    }
  }
  return 0;
}
