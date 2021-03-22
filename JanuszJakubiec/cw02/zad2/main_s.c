#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


const int BUFFER_SIZE = 1000;
const int FILE_NAME_SIZE = 1000;

int main(int argc, char *argv[])
{
  char buffer[BUFFER_SIZE];
  char searched_sign;
  char file[FILE_NAME_SIZE];
  int out = 1;
  char sign[0];
  if(argc < 3)
  {
    char announcement[50] = "Corruption in data detected! \n";
    write(out, announcement, sizeof(char)*strlen(announcement));
    return -1;
  }
  else
  {
    searched_sign = argv[1][0];
    strcpy(file, argv[2]);
  }
  int f = open(file, O_RDONLY);
  if(f == -1)
  {
    char err1[40] = "file not found\n";
    write(out, err1, sizeof(char)*strlen(err1));
    return -1;
  }
  int file1Ended = 0;
  int wsk = 0;
  while(!file1Ended)
  {
    if(read(f, sign, sizeof(char)) == 0)
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
        if(read(f, sign, sizeof(char)) == 0)
        {
          file1Ended = 1;
          break;
        }
      }
      buffer[wsk] = '\n';
      buffer[wsk+1] = '\0';
      if(strchr(buffer, searched_sign) != NULL)
        {
        write(out, buffer, sizeof(char)*(wsk+2));
      }
    }
  }
  close(f);
  return 0;
}
