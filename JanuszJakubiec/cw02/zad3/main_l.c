#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <math.h>

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

void insert_buffor_to_proper_file(char buffer[BUFFER_SIZE], FILE* b, FILE* c, int len)
{
  int val = atoi(buffer);
  buffer[len] = '\n';
  buffer[len+1] = '\0';
  if(buffer[len-2] == '7' || buffer[len-2] == '0')
  {
    fwrite(buffer, sizeof(char), sizeof(char)*(len+1), b);
  }
  int a = sqrt((double)val);
  if(a*a == val || (a+1)*(a+1) == val)
  {
    fwrite(buffer, sizeof(char), sizeof(char)*(len+1), c);
  }
}

int main(int argc, char *argv[])
{
  char buffer[BUFFER_SIZE];
  FILE* out = stdout;
  char sign[0];
  char atext[60] = "Liczb parzystych jest ";
  char even_n[14];
  FILE* f = fopen("dane.txt", "r");
  FILE* a = fopen("a.txt", "w");
  FILE* b = fopen("b.txt", "w");
  FILE* c = fopen("c.txt", "w");
  int even_counter = 0;
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
      buffer[wsk] = '\0';
      even_counter += check_even(buffer, wsk);
      insert_buffor_to_proper_file(buffer, b, c, wsk);
    }
  }
  sprintf(even_n, "%d", even_counter);
  strcat(atext, even_n);
  fwrite(atext, sizeof(char), sizeof(char)*strlen(atext), a);
  fclose(f);
  fclose(a);
  fclose(b);
  fclose(c);
  return 0;
}
