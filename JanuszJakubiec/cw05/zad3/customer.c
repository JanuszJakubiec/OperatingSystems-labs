#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <signal.h>
#include <bits/sigaction.h>

void insert_into_line(char* string, int n, FILE* file)
{
  char sign;
  int counter = 0;
  int flag = 1;
  int k = 0;
  int lic = 0;
  while(fread(&sign, sizeof(char), sizeof(char), file) != 0)
  {
    lic ++;
    if(k == 0)
    k++;
    if(sign == '\n')
    {
      counter++;
      if(counter == n)
      {
        fseek(file, -1, SEEK_CUR);
        flag = 0;
        break;
      }
    }
  }
  if(flag)
  {
    char* buffer = (char*)calloc(strlen(string)+(n-counter)+1,sizeof(char));
    for(int i = 0; i<(n-counter)-1; i++)
    {
      buffer[i] = '\n';
    }
    buffer[n-counter-1] = '\0';
    strcat(buffer, string);
    fwrite(buffer, sizeof(char), strlen(buffer)*sizeof(char), file);
    char enter = '\n';
    fwrite(&enter, sizeof(char), sizeof(char), file);
    if(lic == 0)
    free(buffer);
  }
  else
  {
    FILE * tmp = tmpfile();
    int n = 0;
    while(fread(&sign, sizeof(char), sizeof(char), file) != 0)
    {
      fwrite(&sign, sizeof(char), sizeof(char), tmp);
      n++;
    }
    fseek(file, -n, SEEK_END);
    fseek(tmp, 0, SEEK_SET);
    fwrite(string, sizeof(char), strlen(string)*sizeof(char), file);
    while(fread(&sign, sizeof(char), sizeof(char), tmp) != 0)
    {
      fwrite(&sign, sizeof(char), sizeof(char), file);
    }
    fclose(tmp);
  }
}

int main(int argc, char *argv[])
{
  if(argc < 4)
  {
    printf("%s\n", "Za malo argumentow");
    return -1;
  }
  FILE* pipe = fopen(argv[1], "r");
  int N = atoi(argv[3]);
  char* buffer = (char*)calloc(N+1, sizeof(char));
  char number[10];
  int counter = 0;
  char sign = '\0';
  while(sign != ' ' && fread(&sign, sizeof(char), sizeof(char), pipe) != 0)
  {
    number[counter] = sign;
    counter++;
  }
  number[counter] = '\0';
  while(fread(buffer, sizeof(char), N*sizeof(char), pipe) > 0)
  {
    buffer[N] = '\0';
    counter = 0;
    int n = atoi(number);
    FILE* result = fopen(argv[2], "r+");
    flock(fileno(result), LOCK_EX);
    insert_into_line(buffer, n, result);
    fclose(result);
    sign = '\0';
    while(sign != ' ' && fread(&sign, sizeof(char), sizeof(char), pipe) != 0)
    {
      number[counter] = sign;
      counter++;
    }
    number[counter] = '\0';
  }
  exit(2);
}
