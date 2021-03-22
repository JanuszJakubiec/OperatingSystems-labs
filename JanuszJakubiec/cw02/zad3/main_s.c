#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


const int BUFFER_SIZE = 1000;
const int FILE_NAME_SIZE = 1000;

int check_even(char buffer[BUFFER_SIZE], int len)
{
  char last[1];
  last[0] = buffer[len-1];
  int v = atoi(last);
  if(v%2 == 0)
  {
    return 1;
  }
  return 0;
}

void insert_buffor_to_proper_file(char buffer[BUFFER_SIZE], int b, int c, int len)
{
  int val = atoi(buffer);
  buffer[len] = '\n';
  buffer[len+1] = '\0';
  if(buffer[len-2] == '7' || buffer[len-2] == '0')
  {
    write(b, buffer, sizeof(char)*(len+1));
  }
  int a = sqrt((double)val);
  if(a*a == val || (a+1)*(a+1) == val)
  {
    write(c, buffer, sizeof(char)*(len+1));
  }
}

int main(int argc, char *argv[])
{
  char buffer[BUFFER_SIZE];
  int out = 1;
  char sign[0];
  char atext[60] = "Liczb parzystych jest ";
  char even_n[14];
  int f = open("dane.txt", O_RDONLY);
  int a = open("a.txt", O_WRONLY|O_CREAT, S_IRWXU);
  int b = open("b.txt", O_WRONLY|O_CREAT, S_IRWXU);
  int c = open("c.txt", O_WRONLY|O_CREAT, S_IRWXU);
  int even_counter = 0;
  if(f == -1)
  {
    char err1[40] = "file not found\n";
    write(out, err1, sizeof(char)*strlen(err1));
    return 1;
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
      buffer[wsk] = '\0';
      even_counter += check_even(buffer, wsk);
      insert_buffor_to_proper_file(buffer, b, c, wsk);
    }
  }
  sprintf(even_n, "%d", even_counter);
  strcat(atext, even_n);
  write(a, atext, sizeof(char)*strlen(atext));
  close(f);
  close(a);
  close(b);
  close(c);
  system("chmod 700 a.txt"); //STUPID but WORKS!
  return 0;
}
