#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

const int BUFFER_SIZE = 1000;
const int FILE_NAME_SIZE = 1000;

int main(int argc, char *argv[])
{
  char buffer[BUFFER_SIZE];
  char searched_sign;
  char file[FILE_NAME_SIZE];
  FILE* out = stdout;
  char sign[0];
  if(argc < 3)
  {
    char announcement[50] = "Corruption in data detected! \n";
    fwrite(announcement, sizeof(char), sizeof(char)*strlen(announcement), out);
    return -1;
  }
  else
  {
    searched_sign = argv[1][0];
    strcpy(file, argv[2]);
  }
  FILE* f = fopen(file, "r");
  if(f == NULL)
  {
    char err1[40] = "file not found\n";
    fwrite(err1, sizeof(char), sizeof(char)*strlen(err1), out);
    return 1;
  }
  int file1Ended = 0;
  int wsk = 0;
  while(!file1Ended)
  {
    if(fread(sign, sizeof(char), sizeof(char), f) == 0)
    {
      file1Ended = 1;
    }
    if(!file1Ended)
    {
      wsk = 0;
      while(sign[0] != '\n' && wsk < BUFFER_SIZE - 2)
      {
        buffer[wsk] = sign[0];
        wsk += 1;
        if(fread(sign, sizeof(char), sizeof(char), f) == 0)
        {
          file1Ended = 1;
          break;
        }
      }
      buffer[wsk] = '\n';
      buffer[wsk+1] = '\0';
      if(strchr(buffer, searched_sign) != NULL)
      {
        fwrite(buffer, sizeof(char), sizeof(char)*(wsk+2), out);
      }
    }
  }
  fclose(f);
  return 0;
}
