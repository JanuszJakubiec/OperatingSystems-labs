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

void event_handler(int signum)
{
  printf("%s\n", "event has been handled!");
}

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("%s\n", "Podano zbyt mala liczbe argumentow\n");
    return 0;
  }
  if(strcmp(argv[1], "ignore") == 0)
  {
    struct sigaction act;
    act.sa_handler = SIG_IGN;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);
    if(fork() == 0)
    {
      raise(SIGUSR1);
      exit(0);
    }
  }
  if(strcmp(argv[1], "handler") == 0)
  {
    struct sigaction act;
    act.sa_handler = event_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1);
    if(fork() == 0)
    {
      raise(SIGUSR1);
      exit(0);
    }
  }
  if(strcmp(argv[1], "mask") == 0)
  {
    sigset_t newmask, wait_mask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &newmask, NULL);
    raise(SIGUSR1);
    sigpending(&wait_mask);
    if(sigismember(&wait_mask, SIGUSR1))
    {
      printf("%s\n", "Parent. Signal is pending");
    }
    else
    {
      printf("%s\n", "Parent. There is not such signal");
    }
    if(fork() == 0)
    {
      raise(SIGUSR1);
      sigpending(&wait_mask);
      if(sigismember(&wait_mask, SIGUSR1))
      {
        printf("%s\n", "Child. Signal is pending");
      }
      else
      {
        printf("%s\n", "Child. There is not such signal");
      }
      exit(0);
    }
  }
  if(strcmp(argv[1], "pending") == 0)
  {
    sigset_t newmask, wait_mask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &newmask, NULL);
    raise(SIGUSR1);
    if(fork() == 0)
    {
      sigpending(&wait_mask);
      if(sigismember(&wait_mask, SIGUSR1))
      {
        printf("%s\n", "Child. Signal is pending");
      }
      else
      {
        printf("%s\n", "Child. There is not such signal");
      }
      exit(0);
    }
    wait(NULL);
    sigpending(&wait_mask);
    if(sigismember(&wait_mask, SIGUSR1))
    {
      printf("%s\n", "Parent. Signal is pending");
    }
    else
    {
      printf("%s\n", "Parent. There is not such signal");
    }
  }
  return 0;
}
