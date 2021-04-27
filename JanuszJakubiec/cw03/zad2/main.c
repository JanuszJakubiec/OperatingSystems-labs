#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/wait.h>
#include "file_mergerer.h"

clock_t start, end;
struct tms tms_start, tms_end;

void delegate_proccess_to_merge_files(struct files_list_header list)
{
  if(fork() == 0)
  {
    struct tmp_files_header files;
    files.size = 0;
    files.table = NULL;
    merge_many_files(list, files);
    delete_tmp_files(files);
    //printf("%s%d\n", "Jestem procesem potomnym, moj PID to: ", (int)getpid());
    exit(0);
  }
}

int main(int argc, char *argv[])
{
  if(argc < 1)
    return 1;
  struct files_list_header list;
  list.files_list = (char**)calloc(1, sizeof(char*));
  list.size = 1;
  list.files_list[0] = (char*)calloc(100, sizeof(char));
  start = times(&tms_start);
  for(int i = 0; i<argc-1; i++)
  {
    strcpy(list.files_list[0],argv[i+1]);
    delegate_proccess_to_merge_files(list);
  }
  for(int i = 0; i<argc-1; i++)
  {
    wait(NULL);
  }
  end = times(&tms_end);
  printf("%s\n", "merging:");
  printf("real time: %f\n", (double)(end - start)/sysconf(_SC_CLK_TCK));
  printf("user time: %f\n", (double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
  printf("system time: %f\n", (double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK));
  free_list(list);
  return 0;
}
