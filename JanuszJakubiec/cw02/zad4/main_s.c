#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void find_and_replace(char* buffer, char* n1, char* n2, int file)
{
  int buffer_l = strlen(buffer);
  int n1_len = strlen(n1);
  int n2_len = strlen(n2);
  for(int i = 0; i<buffer_l; i++)
  {
    if(buffer[i] == n1[0])
    {
      int j = 1;
      for(j = 1; j<n1_len;j++)
      {
        if(buffer[i+j] != n1[j] || i+j >= buffer_l)
          break;
      }
      if(j == n1_len)
      {
        i += j-1;
        for(j = 0; j<n2_len;j++)
        {
          write(file, &(n2[j]), sizeof(char));
        }
      }
      else
      {
        write(file, &(buffer[i]), sizeof(char));
      }
    }
    else
    {
      write(file, &(buffer[i]), sizeof(char));
    }
  }
}

void function(char* file_r, char* file_w, char* n1, char* n2)
{
  char* buffer;
  char sign[0];
  int out = 1;
  int f_r = open(file_r, O_RDONLY);
  int f_w = open(file_w, O_WRONLY);
  if(f_r == -1)
  {
    char err1[40] = "file not found\n";
    write(out, err1, sizeof(char)*strlen(err1));
    return;
  }
  int len;
  len = lseek(f_r, 0, SEEK_END);
  lseek(f_r, 0, SEEK_SET);
  buffer = (char*)calloc(len+1, sizeof(char));
  for(int i = 0; i < len; i++)
  {
    read(f_r, sign, sizeof(char));
    buffer[i]=sign[0];
  }
  find_and_replace(buffer, n1, n2, f_w);
  close(f_r);
  close(f_w);
  free(buffer);
}
int main()
{
  function("a", "b", "b\naa", "gg");
}
