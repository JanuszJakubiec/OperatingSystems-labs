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
    raise(SIGUSR1);
    exit(0);
  }
  if(strcmp(argv[1], "mask") == 0)
  {
    sigset_t wait_mask;
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
  }
  if(strcmp(argv[1], "pending") == 0)
  {
    sigset_t wait_mask;
    sigpending(&wait_mask);
    if(sigismember(&wait_mask, SIGUSR1))
    {
      printf("%s\n", "Child. Signal is pending");
    }
    else
    {
      printf("%s\n", "Child. There is not such signal");
    }
  }
  return 0;
}
