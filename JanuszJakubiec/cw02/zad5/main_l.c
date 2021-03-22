#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>

const int BUFFER_SIZE = 60;
const int FILE_NAME_SIZE = 1000;

int main(int argc, char *argv[])
{
  char buffer[BUFFER_SIZE];
  char file_w[FILE_NAME_SIZE];
  char file_r[FILE_NAME_SIZE];
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
    strcpy(file_r, argv[1]);
    strcpy(file_w, argv[2]);
  }
  FILE* f_r = fopen(file_r, "r");
  FILE* f_w = fopen(file_w, "w");
  if(f_r == NULL)
  {
    char err1[40] = "file not found\n";
    fwrite(err1, sizeof(char), sizeof(char)*strlen(err1), out);
    return 1;
  }
  int file1Ended = 0;
  int wsk = 0;
  while(!file1Ended)
  {
    if(fread(sign, sizeof(char), sizeof(char), f_r) == 0)
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
        if(fread(sign, sizeof(char), sizeof(char), f_r) == 0)
        {
          file1Ended = 1;
          break;
        }
        if(wsk == 50 && sign[0] != '\n')
        {
          buffer[wsk] = '\n';
          buffer[wsk+1] = '\0';
          fwrite(buffer, sizeof(char), sizeof(char)*strlen(buffer), f_w);
          wsk = 0;
        }
      }
      buffer[wsk] = '\n';
      buffer[wsk+1] = '\0';
      fwrite(buffer, sizeof(char), sizeof(char)*strlen(buffer), f_w);
    }
  }
  fclose(f_r);
  fclose(f_w);
  return 0;
}
