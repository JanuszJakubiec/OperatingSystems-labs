#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("%s\n", "Podano zbyt mala liczbe argumentow\n");
    return 0;
  }
  int n = atoi(argv[1]);
  for(int i = 0; i < n; i++)
  {
    pid_t proc_tab = fork();
    if(proc_tab == 0)
    {
      printf("%s%d\n", "Jestem procesem potomnym, moj PID to: ", (int)getpid());
      return 0;
    }
  }
  return 0;
}
