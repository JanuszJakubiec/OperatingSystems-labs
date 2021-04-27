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
//#include <sys/siginfo.h>

int sigque = 0;
int i = 0;
pid_t sender_pid;
int stop = 0;

void usr1_handler(int sig, siginfo_t *siginfo, void *context)
{
  if(i == 0)
  {
    sender_pid = siginfo -> si_pid;
  }
  i = i+1;
  if(sigque == 1)
  {
    union sigval value;
    value.sival_int = i;
    sigqueue(sender_pid, SIGUSR1,value);
  }
}

void usr2_handler(int sig, siginfo_t *siginfo, void *context)
{
  printf("%s %d %s\n", "Odebralem", i, "sygnalow!");
  stop = 1;
}

int main(int argc, char *argv[])
{
  if(argc < 1)
  {
    printf("%s\n", "ZA MALO ARUGUMENTOW");
    return 0;
  }
  if(strcmp(argv[1], "SIGQUEUE") == 0)
  {
    sigque = 1;
  }
  printf("%d\n", getpid());
  sigset_t newmask, block_mask;
  sigfillset(&block_mask);
  sigfillset(&newmask);
  struct sigaction act1, act2;
  sigemptyset(&act1.sa_mask);
  act1.sa_flags = SA_SIGINFO;
  act1.sa_sigaction = usr1_handler;
  sigemptyset(&act2.sa_mask);
  act2.sa_sigaction = usr2_handler;
  act2.sa_flags = SA_SIGINFO;
  if(strcmp(argv[1], "SIGRT") != 0)
  {
    sigdelset(&newmask, SIGUSR1);
    sigdelset(&newmask, SIGUSR2);
    sigaddset(&act1.sa_mask, SIGUSR2);
    sigaddset(&act2.sa_mask, SIGUSR1);
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
    sigaction(SIGRTMIN+1, &act1, NULL);
    sigaction(SIGRTMIN+2, &act2, NULL);
    sigdelset(&block_mask, SIGRTMIN+1);
    sigdelset(&block_mask, SIGRTMIN+2);
  }
  sigprocmask(SIG_BLOCK, &block_mask, NULL);
  while(stop == 0)
  {
    sigsuspend(&newmask);
  }
  if(strcmp(argv[1], "kill") == 0)
  {
    for(int j = 0; j<i; j++)
    {
      if(kill(sender_pid, SIGUSR1) == -1)
      {
        printf("%s\n", "NO PROGRAM WITH THAT PID!");
        return -1;
      }
    }
    kill(sender_pid, SIGUSR2);
  }
  if(strcmp(argv[1], "SIGQUEUE") == 0)
  {
    union sigval value;
    for(int j = 0; j<i; j++)
    {
      if(sigqueue(sender_pid, SIGUSR1, value) != 0)
      {
        printf("%s\n", "NO PROGRAM WITH THAT PID!");
        return -1;
      }
    }
    sigqueue(sender_pid, SIGUSR2, value);
  }
  if(strcmp(argv[1], "SIGRT") == 0)
  {
    for(int j = 0; j<i; j++)
    {
      if(kill(sender_pid,SIGRTMIN+1) != 0)
      {
        printf("%s\n", "NO PROGRAM WITH THAT PID!");
        return -1;
      }
    }
    kill(sender_pid,SIGRTMIN+2);
  }

  return 0;
}
