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
    execl("./child", "child", "ignore", NULL);
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
    execl("./child", "child", "mask", NULL);
  }
  if(strcmp(argv[1], "pending") == 0)
  {
    sigset_t newmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &newmask, NULL);
    raise(SIGUSR1);
    execl("./child", "child", "pending", NULL);
  }
  return 0;
}
