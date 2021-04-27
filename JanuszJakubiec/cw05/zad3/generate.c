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
  srand(time(NULL));
  static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
  system("touch proper.txt");
  for(int i = 1; i<atoi(argv[1])+1; i++)
  {
    char command[100] = "";
    char integer[15];
    sprintf(integer, "%d", i);
    strcat(integer, ".txt");
    strcat(command, "cat ");
    strcat(command, integer);
    char buffer[30000];
    for(int i = 0; i<atoi(argv[2]); i++)
    {
      int key = rand() % (int) (sizeof charset - 1);
      buffer[i] = charset[key];
    }
    FILE * file = fopen(integer, "w");
    fwrite(buffer, sizeof(char), atoi(argv[2])*sizeof(char), file);
    fclose(file);
    strcat(command, " >> proper.txt");
    system(command);
    system("echo \"\" >> proper.txt");
  }
  exit(2);
}
