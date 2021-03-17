#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include "file_mergerer.h"

clock_t start, end;
struct tms tms_start, tms_end;

int find_size_of_files_list(char *argv[], int i, int s)
{
  int n = 0;
  while(i<s && strstr(argv[i], ":") != NULL)
  {
    n = n+1;
    i = i+1;
  }
  return n;
}

int main(int argc, char *argv[])
{
  int real = 0;
  int sys = 0;
  int user = 0;
  struct files_list_header list;
  struct tmp_files_header tmp_files;
  tmp_files.size = 0;
  tmp_files.table = NULL;
  list.files_list = NULL;
  list.size = 0;
  int print_merging = 1;
  int print_saving = 1;
  int print_deleting = 1;
  if(argc < 1)
    return 1;
  int number = atoi(argv[1]);
  struct main_table_header header = create_table(number);
  for(int i = 2; i<argc; i++)
  {
    if(strcmp(argv[i], "merge_files") == 0)
    {
      free_list(list);
      int n = find_size_of_files_list(argv, i+1, argc);
      list.size = n;
      list.files_list = (char**)calloc(n, sizeof(char*));
      int j = i;
      for(int i = j+1; i<j+n+1;i++)
      {
        list.files_list[i-j-1] = (char*)calloc(100, sizeof(char));
        strcpy(list.files_list[i-j-1],argv[i]);
      }
      start = times(&tms_start);
      tmp_files = merge_many_files(list, tmp_files);
      end = times(&tms_end);
      real += end - start;
      user += tms_end.tms_utime - tms_start.tms_utime;
      sys += tms_end.tms_stime - tms_start.tms_stime;
      if(print_merging)
      {
        printf("%s\n", "merging:");
        printf("real time: %f\n", (double)(end - start)/sysconf(_SC_CLK_TCK));
        printf("user time: %f\n", (double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
        printf("system time: %f\n", (double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK));
        print_merging = 0;
      }

    }
    if(strcmp(argv[i], "remove_block") == 0)
    {
      start = times(&tms_start);
      header = delete_block(header, atoi(argv[i+1]));
      end = times(&tms_end);
      real += end - start;
      user += tms_end.tms_utime - tms_start.tms_utime;
      sys += tms_end.tms_stime - tms_start.tms_stime;
      if(print_deleting)
      {
        printf("%s\n", "delete_block:");
        printf("real time: %f\n", (double)(end - start)/sysconf(_SC_CLK_TCK));
        printf("user time: %f\n", (double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
        printf("system time: %f\n", (double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK));
        print_deleting = 0;
      }
      i = i+1;
    }
    if(strcmp(argv[i], "remove_row") == 0)
    {
      header = delete_row_from_table(header, atoi(argv[i+1]), atoi(argv[i+2]));
      i = i + 2;
    }
    if(strcmp(argv[i], "print_block") == 0)
    {
      print_block(header, atoi(argv[i+1]));
      i = i + 1;
    }
    if(strcmp(argv[i], "save_data") == 0)
    {
      start = times(&tms_start);
      header = save_tmp_to_table(&tmp_files, header);
      end = times(&tms_end);
      real += end - start;
      user += tms_end.tms_utime - tms_start.tms_utime;
      sys += tms_end.tms_stime - tms_start.tms_stime;
      if(print_saving)
      {
        printf("%s\n", "saving:");
        printf("real time: %f\n", (double)(end - start)/sysconf(_SC_CLK_TCK));
        printf("user time: %f\n", (double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
        printf("system time: %f\n", (double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK));
        print_saving = 0;
      }
    }
    if(strcmp(argv[i], "get_rows_number") == 0)
    {
      printf("%d\n", get_number_of_rows(header, atoi(argv[i+1])));
      i = i+1;
    }
  }

  printf("%s\n", "many operations duration time:" );
  printf("real time: %f\n", (double)(real)/sysconf(_SC_CLK_TCK));
  printf("user time: %f\n", (double)(user)/sysconf(_SC_CLK_TCK));
  printf("system time: %f\n", (double)(sys)/sysconf(_SC_CLK_TCK));
  free_memory(header);
  free_list(list);
  delete_tmp_files(tmp_files);
  return 0;
}
