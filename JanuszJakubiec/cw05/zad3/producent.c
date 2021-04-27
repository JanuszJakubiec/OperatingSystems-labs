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
#include <fcntl.h>
#include <bits/sigaction.h>

int main(int argc, char *argv[])
{
  srand(time(NULL));
  if(argc < 5)
  {
    printf("%s\n", "Za malo argumentow");
    return -1;
  }
  int output = open(argv[1], O_WRONLY|S_IFIFO);
  if(output < 0)
  {
    printf("%s\n", "there is no pipe");
    return 0;
  }
  int input = open(argv[3], O_RDONLY);
  if(input < 0)
  {
    close(output);
    printf("%s\n", "there is no read file");
    return 0;
  }
  int N = atoi(argv[4]);
  char* buffer = (char*)calloc(N+1+strlen(argv[2]), sizeof(char));
  char* buf = (char*)calloc(N+1, sizeof(char));
  while(read(input, buf, N*sizeof(char))!=0)
  {
    buffer[0] = '\0';
    int n = 1000*((rand()%1000)+1000);
    strcat(buffer, argv[2]);
    strcat(buffer, " ");
    int l = strlen(buf);
    if(buf[l-1] == '\n')
    {
      buf[l-1] = '\0';
    }
    strcat(buffer, buf);
    write(output, buffer, (N+1+strlen(argv[2]))*sizeof(char));
    usleep(n);
    for(int i = 0; i< N+strlen(argv[2]) + 1; i++)
    {
      buf[i] = '\0';
    }
  }
  free(buffer);
  close(input);
  close(output);
}
