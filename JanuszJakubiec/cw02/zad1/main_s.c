#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

const int BUFFER_SIZE = 10000;
const int FILE_NAME_SIZE = 1000;

int main(int argc, char *argv[])
{
  char buffer[BUFFER_SIZE];
  char file1[FILE_NAME_SIZE];
  char file2[FILE_NAME_SIZE];
  int out = 0;
  int in = 1;
  char sign[1];
  if(argc < 3)
  {
    char announcement[50] = "Please insert files names: \n";
    write(out, announcement, sizeof(char)*strlen(announcement));
    read(in, sign, sizeof(char));
    int i = 0;
    while(sign[0] != ' ' && sign[0] != '\n' && i < FILE_NAME_SIZE)
    {
      file1[i] = sign[0];
      i = i + 1;
      read(in, sign, sizeof(char));
    }
    file1[i] = '\0';
    i = 0;
    read(in, sign, sizeof(char));
    while(sign[0] != ' ' && sign[0] != '\n' && i < FILE_NAME_SIZE)
    {
      file2[i] = sign[0];
      i = i + 1;
      read(in, sign, sizeof(char));
    }
    file2[i] = '\0';
  }
  else
  {
    strcpy(file1, argv[1]);
    strcpy(file2, argv[2]);
  }
  int f1 = open(file1, O_RDONLY);
  int f2 = open(file2, O_RDONLY);
  if(f1 == -1 && f2 == -1)
  {
    char err1[40] = "file1 and file2 not found\n";
    write(out, err1, strlen(err1)*sizeof(char));
    return -1;
  }
  if(f1 == -1)
  {
    close(f2);
    char err1[40] = "file1 not found\n";
    write(out, err1, strlen(err1)*sizeof(char));
    return -1;
  }
  if(f2 == -1)
  {
    close(f1);
    char err1[40] = "file2 not found\n";
    write(out, err1, strlen(err1)*sizeof(char));
    return -1;
  }
  int file1Ended = 0;
  int file2Ended = 0;
  int wsk = 0;
  while(!file1Ended || !file2Ended)
  {
    if(read(f1, sign, sizeof(char)) == 0)
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
        if(read(f1, sign, sizeof(char)) == 0)
        {
          file1Ended = 1;
          break;
        }
      }
      buffer[wsk] = '\n';
      buffer[wsk+1] = '\0';
      write(out, buffer, sizeof(char)*(wsk+2));
    }
    if(read(f2, sign, sizeof(char)) == 0)
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
        if(read(f2, sign, sizeof(char)) == 0)
        {
          file1Ended = 1;
          break;
        }
      }
      buffer[wsk] = '\n';
      buffer[wsk+1] = '\0';
      write(out, buffer, sizeof(char)*(wsk+2));
    }
  }
  close(f1);
  close(f2);
  return 0;
}
