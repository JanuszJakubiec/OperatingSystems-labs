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
#include <sys/wait.h>


int main(int argc, char *argv[])
{
  if(argc < 4)
  {
    printf("%s\n", "Za malo argumentow");
    return -1;
  }
  mkfifo("zad3pipe", S_IRUSR | S_IWUSR);
  pid_t pid = fork();
  if(pid == 0)
  {
    execl("./generate", "generate", argv[2], argv[4], NULL);
  }
  waitpid(pid, NULL, WIFEXITED(2));
  for(int i = 1; i<=atoi(argv[2]); i++)
  {
    if(fork() == 0)
    {
      char procname[20] = "producent";
      char integer[15];
      char integer2[15];
      sprintf(integer, "%d", i);
      sprintf(integer2, "%d", i);
      strcat(procname, integer);
      strcat(integer2, ".txt");
      execl("./producent", procname, "zad3pipe", integer, integer2, argv[1], NULL);
    }
  }
  if(atoi(argv[3]) > 1)
  {
    for(int i = 0; i<atoi(argv[3]); i++)
    {
      if(fork() == 0)
      {
        char procname[20] = "customer";
        char integer[15];
        sprintf(integer, "%d", i);
        strcat(procname, integer);
        execl("customer", procname, "zad3pipe", "result.txt", argv[1], NULL);
        exit(0);
      }
    }
    int n = atoi(argv[2]) + atoi(argv[3]);
    for(int i = 0; i<n; i++)
    {
      wait(NULL);
    }
  }
  else
  {
    pid_t customer = fork();
    if(customer == 0)
    {
      execl("customer", "customer", "zad3pipe", "result.txt", argv[1], NULL);
    }
    waitpid(customer, NULL, WIFEXITED(2));
  }
  FILE* result = popen("diff proper.txt result.txt", "r");
  char sign;
  if(fread(&sign, sizeof(char), sizeof(char), result) == 0)
  {
    printf("%s\n", "Matched perfectly");
  }
  else
  {
    printf("%s\n", "Something is wrong");
  }
}
