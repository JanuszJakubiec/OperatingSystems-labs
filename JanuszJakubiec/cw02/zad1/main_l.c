#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

const int BUFFER_SIZE = 10000;
const int FILE_NAME_SIZE = 1000;

int main(int argc, char *argv[])
{
  char buffer[BUFFER_SIZE];
  char file1[FILE_NAME_SIZE];
  char file2[FILE_NAME_SIZE];
  FILE* out = stdout;
  FILE* in = stdin;
  char sign[1];
  if(argc < 3)
  {
    char announcement[50] = "Please insert files names: \n";
    fwrite(announcement, sizeof(char), sizeof(char)*strlen(announcement), out);
    fread(sign, sizeof(char), sizeof(char), in);
    int i = 0;
    while(sign[0] != ' ' && sign[0] != '\n' && i < FILE_NAME_SIZE)
    {
      file1[i] = sign[0];
      i = i + 1;
      fread(sign, sizeof(char), sizeof(char), in);
    }
    file1[i] = '\0';
    i = 0;
    fread(sign, sizeof(char), sizeof(char), in);
    while(sign[0] != ' ' && sign[0] != '\n' && i < FILE_NAME_SIZE)
    {
      file2[i] = sign[0];
      i = i + 1;
      fread(sign, sizeof(char), sizeof(char), in);
    }
    file2[i] = '\0';
  }
  else
  {
    strcpy(file1, argv[1]);
    strcpy(file2, argv[2]);
  }
  FILE* f1 = fopen(file1, "r");
  FILE* f2 = fopen(file2, "r");
  if(f1 == NULL && f2 == NULL)
  {
    char err1[40] = "file1 and file2 not found\n";
    fwrite(err1, sizeof(char), sizeof(char)*strlen(err1), out);
    return -1;
  }
  if(f1 == NULL)
  {
    fclose(f2);
    char err1[40] = "file1 not found\n";
    fwrite(err1, sizeof(char), sizeof(char)*strlen(err1), out);
    return -1;
  }
  if(f2 == NULL)
  {
    fclose(f1);
    char err1[40] = "file2 not found\n";
    fwrite(err1, sizeof(char), sizeof(char)*strlen(err1), out);
    return -1;
  }
  int file1Ended = 0;
  int file2Ended = 0;
  int wsk = 0;
  while(!file1Ended || !file2Ended)
  {
    if(fread(sign, sizeof(char), sizeof(char), f1) == 0)
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
        if(fread(sign, sizeof(char), sizeof(char), f1) == 0)
        {
          file1Ended = 1;
          break;
        }
      }
      buffer[wsk] = '\n';
      buffer[wsk+1] = '\0';
      fwrite(buffer, sizeof(char), sizeof(char)*(wsk+2), out);
    }
    if(fread(sign, sizeof(char), sizeof(char), f2) == 0)
    {
      file2Ended = 1;
    }
    if(!file2Ended)
    {
      wsk = 0;
      while(sign[0] != '\n' && wsk < BUFFER_SIZE - 2)
      {
        buffer[wsk] = sign[0];
        wsk += 1;
        if(fread(sign, sizeof(char), sizeof(char), f2) == 0)
        {
          file1Ended = 1;
          break;
        }
      }
      buffer[wsk] = '\n';
      buffer[wsk+1] = '\0';
      fwrite(buffer, sizeof(char), sizeof(char)*(wsk+2), out);
    }
  }
  fclose(f1);
  fclose(f2);
  return 0;
}
