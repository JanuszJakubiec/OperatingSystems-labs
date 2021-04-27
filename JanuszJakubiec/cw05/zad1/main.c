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
#include <bits/sigaction.h>

#define SIZE 1000

char** get_args(char* prog, int* size)
{
  char* buf = (char*)calloc(SIZE, sizeof(char));
  strcpy(buf, prog);
  int j = 0;
  char* tmp = strtok_r(buf, " ", &buf);
  while(tmp!=NULL)
  {
    j++;
    tmp = strtok_r(buf, " ", &buf);
  }
  char** args = (char**)calloc(j, sizeof(char*));
  for(int i = 0; i<j; i++)
  {
    args[i] = strtok_r(prog," ", &prog);
  }
  *size = j;
  return args;
}

void run_line(char* string, int n)
{
  int size = 0;
  int** pipes;
  pipes = (int**)calloc(n-1, sizeof(int*));
  for(int i = 0; i<n; i++)
  {
    pipes[i] = (int*)calloc(2, sizeof(int));
    if(pipe(pipes[i]) < 0)
    {
      printf("%s\n", "Could not create pipe");
      exit(0);
    }
  }
  for(int i = 0; i < n; i++)
  {
    char* prog = strtok_r(string, "|", &string);
    char** args = get_args(prog, &size);
    if(fork() == 0)
    {
      if(i > 0)
      {
        dup2(pipes[i-1][0], STDIN_FILENO);
      }
      if(i+1 < n)
      {
        dup2(pipes[i][1], STDOUT_FILENO);
      }
      for(int k=0;k<n;k++)
      {
        if(k != i)
        {
          close(pipes[k][0]);
          close(pipes[k][1]);
        }
      }
      execvp(args[0], args);
      exit(0);
    }
    free(args);
  }
  for(int i = 0; i<n; i++)
  {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }
  for(int i = 0; i<n; i++)
  {
    wait(NULL);
  }
  for(int i = 0; i<n-1; i++)
  {
    free(pipes[i]);
  }
  free(pipes);
}

int get_number(FILE* file)
{
  int counter = 0;
  int n = 0;
  char sign;
  while(counter < 2)
  {
    if(fread(&sign, sizeof(char), sizeof(char), file) == 0)
    {
      printf("%s\n", "FIle ended unexpectedly");
      exit(-1);
    }
    if(sign == '\n')
    {
      counter++;
      n++;
    }
    else
    {
      counter = 0;
    }
  }
  rewind(file);
  return n-1;
}

void fill_table_with_components(FILE* file, char** tab, int size, int* tab_w)
{
  int i = 0;
  while(i<size)
  {
    char sign = '\0';
    int counter = 0;
    while(sign != '=')
    {
      if(fread(&sign, sizeof(char), sizeof(char), file) == 0)
      {
        printf("%s\n", "FIle ended unexpectedly");
        exit(-1);
      }
    }
    fread(&sign, sizeof(char), sizeof(char), file);
    if(sign != ' ')
    {
      tab[i][counter] = sign;
      counter++;
    }
    while(sign != '\n' && counter<SIZE)
    {
      if(fread(&sign, sizeof(char), sizeof(char), file) == 0)
      {
        printf("%s\n", "FIle ended unexpectedly");
        exit(-1);
      }
      tab[i][counter] = sign;
      if(sign == '|')
      {
        tab_w[i]++;
      }
      counter++;
    }
    tab[i][counter-1] = '\0';
    tab_w[i]++;
    i++;
  }
}

void process_commands(FILE* file, char** tab, int* tab_w)
{
  char res[10000] = "";
  char num[10];
  char sign = '\0';
  int counter = 0;
  int flag = 1;
  while(flag == 1)
  {
    int n = 0;
    while(sign != '\n')
    {
      int count = 0;
      while(sign > '9' || sign < '0')
      {
        if(fread(&sign, sizeof(char), sizeof(char), file) == 0)
        {
          sign = '\n';
          flag = 0;
          break;
        }
      }
      if(flag == 0)
      {
        break;
      }
      while(sign >= '0' && sign <= '9')
      {
        num[count] = sign;
        count++;
        if(fread(&sign, sizeof(char), sizeof(char), file) == 0)
        {
          sign = '\n';
          flag = 0;
        }
      }
      if(flag == 0)
      {
        break;
      }
      num[count] = '\0';
      int number = atoi(num);
      n += tab_w[number-1];
      strcat(res, tab[number-1]);
      if(sign != '\n')
        strcat(res, " | ");
      counter++;
    }
    if(flag != 0)
    {
      run_line(res, n);
    }
    counter = 0;
    strcpy(res, "");
    sign = '\0';
  }
}

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("%s\n", "Podano zbyt mala liczbe argumentow\n");
    return 0;
  }
  FILE* source = fopen(argv[1], "r");
  if(source == NULL)
  {
    printf("%s\n", "No input file\n");
    return -1;
  }
  int size = get_number(source);
  char ** tab = (char**)calloc(size, sizeof(char*));
  int* tab_w =(int*)calloc(size, sizeof(int));
  for(int i =0; i<size;i++)
  {
    tab[i] = (char*)calloc(SIZE, sizeof(char));
  }
  fill_table_with_components(source,tab,size,tab_w);
  process_commands(source, tab,tab_w);
  for(int i =0; i<size;i++)
  {
    free(tab[i]);
  }
  free(tab_w);
  free(tab);
}
