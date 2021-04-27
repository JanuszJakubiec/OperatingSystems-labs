#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

clock_t start, end;
struct tms tms_start, tms_end;

int check_file(char* filename, char* searched_string)
{
  char sign[0];
  char* buff;
  int size = strlen(searched_string);
  buff = (char*)calloc(size, sizeof(char));
  FILE* file = fopen(filename, "r");
  if(file == NULL)
    return 0;
  int i = 0;
  int k = 0;
  int can_compare = 0;
  while(fread(sign, sizeof(char), sizeof(char), file))
  {
    buff[i%size] = sign[0];
    k = (k + 1)%size;
    if(i == size-1)
      can_compare = 1;
    if(can_compare)
    {
      int j;
      for(j = 0; j<size; j++)
      {
        if(buff[(j+k)%size] != searched_string[j])
          break;
      }
      if(j == size)
      {
        free(buff);
        fclose(file);
        return 1;
      }
    }
    i = i + 1;
  }
  free(buff);
  fclose(file);
  return 0;
}

void process_dir(int left_depth, char* path, char* searched_string, char* path_from_start)
{
  char buffer[1000];
  char buffer2[1000];
  DIR* dir = opendir(path);
  if(dir == NULL)
  {
    printf("FAILED TO OPEN DIR\n");
    exit(-1);
  }
  struct dirent* file = readdir(dir);
  while(file != NULL)
  {
    if(!strcmp(file->d_name, ".") || !strcmp(file->d_name, ".."))
    {
      file = readdir(dir);
      continue;
    }
    strcpy(buffer, path);
    strcat(buffer, "/");
    strcat(buffer, file->d_name);
    if(file -> d_type == 4)
    {
      if(left_depth > 0)
      {
        strcpy(buffer2,path_from_start);
        if(strlen(path_from_start) > 0)
          strcat(buffer2,"/");
        strcat(buffer2, file -> d_name);
        if(fork() == 0)
        {
          process_dir(left_depth - 1, buffer, searched_string, buffer2);
          exit(0);
        }
      }
    }
    else
    {
      if(strlen(file -> d_name) >4)
      {
        char end[5];
        int len = strlen(file->d_name);
        for(int i  = len - 4;i<len+1;i++)
        {
          end[i - len + 4] = file->d_name[i];
        }
        if(!strcmp(end, ".txt"))
        {
          if(check_file(buffer, searched_string))
          {
            if(strlen(path_from_start) > 0)
              printf("%s/%s PID: %d\n",path_from_start, file->d_name, (int)getpid());
            else
              printf("%s PID: %d\n",file->d_name, (int)getpid());
          }
        }
      }
    }
    file = readdir(dir);
  }
}

int main(int argc, char *argv[])
{
  if(argc < 3)
  {
    printf("TOO FEW ARGUMENTS\n");
    return 1;
  }
  if(fork() == 0)
  {
    process_dir(atoi(argv[3]), argv[1], argv[2], "");
    exit(0);
  }
  return 0;
}
