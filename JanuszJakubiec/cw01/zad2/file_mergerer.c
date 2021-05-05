#include "file_mergerer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct tmp_files_header delete_tmp_files(struct tmp_files_header header)
{
  if(header.table == NULL)
    return header;
  for(int i=0; i<header.size; i++)
  {
    fclose(header.table[i]);
  }
  free(header.table);
  header.table = NULL;
  return header;
}

struct main_table_header create_table(int n)
{
  struct main_table_header main_table;
  main_table.last_available = 0;
  main_table.blocks_n = n;
  main_table.table = (struct rows_table_header*)calloc(n, sizeof(struct rows_table_header));
  return main_table;
}

FILE* merge_two_files(char file1[100], char file2[100])
{
  FILE * first;
  FILE * second;
  first = fopen (file1,"r");
  second = fopen (file2,"r");

  if(!first && !second)
  {
    printf("%s%s%s%s%s\n", "FILE ", file1," AND ", file2," DON'T NOT EXIST");
    return NULL;
  }
  if(!first)
  {
    printf("%s%s%s\n", "FILE ", file1, " DOES NOT EXIST");
    fclose(second);
    return NULL;
  }
  if(!second)
  {
    printf("%s%s%s\n", "FILE ", file2, " DOES NOT EXIST");
    fclose(first);
    return NULL;
  }
  char buffer[10001];
  FILE * tmp = tmpfile();
  int file1NotEnded = 1;
  int file2NotEnded = 1;
  //reading file1 and file2, and merging them into tmp file
  //I'm also counting lines in order to use it in the process of creating rows-table
  while(file1NotEnded == 1 || file2NotEnded == 1)
  {
    if(file1NotEnded == 1)
    {
      if(fgets(buffer, 10000, first) != NULL)
      {
        fputs(buffer, tmp);
      }
      else
      {
        file1NotEnded = 0;
      }
    }
    if(file2NotEnded == 1)
    {
      if(fgets(buffer, 10000, second) != NULL)
      {
        fputs(buffer, tmp);
      }
      else
      {
        file2NotEnded = 0;
      }
    }
  }
  fclose(first);
  fclose(second);
  return tmp;
}

struct tmp_files_header merge_many_files(struct files_list_header files, struct tmp_files_header tmp_files)
{

  if(tmp_files.size > 0)
    tmp_files = delete_tmp_files(tmp_files);
  char file1[100];
  char file2[100];
  char* colon;
  tmp_files.table = (FILE**)calloc(files.size, sizeof(FILE*));
  tmp_files.size = 0;
  for(int i=0;i<files.size;i++)
  {
    tmp_files.size += 1;
    colon = strchr(files.files_list[i], ':');
    int index = (int)(colon - files.files_list[i]);
    strncpy(file1, files.files_list[i], index);
    file1[index] = '\0';
    strncpy(file2, files.files_list[i]+index+1, strlen(files.files_list[i]));
    tmp_files.table[i] = merge_two_files(file1, file2);
    if(tmp_files.table[i] == NULL)
    {
      if(tmp_files.size > 0)
      {
        tmp_files.size -=1;
        tmp_files = delete_tmp_files(tmp_files);
      }
      tmp_files.size = 0;
      free(tmp_files.table);
      return tmp_files;
    }
  }
  return tmp_files;
}

struct rows_table_header save_single_file(FILE * tmp)
{
  rewind(tmp); //start reading tmp from the beginning
  char buffer[10001];
  int n = 0;
  while(fgets(buffer, 10000, tmp) != NULL)
  {
    n = n+1;
  }
  rewind(tmp); //start reading tmp from the beginning
  struct rows_table_header rows_table;
  rows_table.rows_n = n;
  rows_table.table = (char**)calloc(n, sizeof(char*));
  rows_table.deleted = 0;
  for(int i = 0; i<n;i++)
  {
    fgets(buffer, 10000, tmp);
    int len = strlen(buffer);
    rows_table.table[i] = (char*)calloc(len+1, sizeof(char));
    strcpy(rows_table.table[i], buffer);
  }
  fclose(tmp);
  return rows_table;
}

struct main_table_header save_tmp_to_table(struct tmp_files_header* files_header, struct main_table_header header)
{
  if(files_header->size == 0)
    return header;
  for(int i=0;i<files_header->size;i++)
  {
    if(header.last_available == header.blocks_n)
    {
      printf("%s\n", "SIZE OF THE MAIN TABLE WAS OVERFLOWED, PROGRAMM DIDN'T SAVE ALL DATA");
      for(int j = i;j<files_header->size;j++)
      {
        fclose(files_header->table[j]);
      }
      free(files_header->table);
      files_header->size = 0;
      return header;
    }
    header.table[header.last_available] = save_single_file(files_header->table[i]);
    header.last_available = header.last_available + 1;
  }
  files_header->size = 0;
  free(files_header->table);
  files_header->table = NULL;
  return header;
}

int get_number_of_rows(struct main_table_header header, int block_n)
{
  struct rows_table_header rows_table = header.table[block_n];
  return rows_table.rows_n - rows_table.deleted;
}

struct main_table_header delete_row_from_table(struct main_table_header header, int block_n, int row_n)
{
  if(block_n >= header.last_available)
  {
    printf("%s\n", "REQUESTED DELETION IN UNEXISTING BLOCK" );
    return header;
  }
  if(row_n >= header.table[block_n].rows_n)
  {
    printf("%s\n", "REQUESTED DELETION OF UNEXISTING ROW" );
    return header;
  }
  if(header.table[block_n].table[row_n] == NULL)
  {
    printf("%s\n", "REQUESTED DELETION OF UNEXISTING ROW" );
    return header;
  }
  free(header.table[block_n].table[row_n]);
  header.table[block_n].table[row_n]=NULL;
  header.table[block_n].deleted = header.table[block_n].deleted + 1;
  return header;
}

struct main_table_header delete_block(struct main_table_header header, int n)
{
  if(n >= header.last_available)
  {
    printf("%s\n", "REQUESTED DELETION OF UNEXISTING BLOCK" );
    return header;
  }
  struct rows_table_header block = header.table[n];
  for(int i=0;i<block.rows_n;i++)
  {
    free(block.table[i]);
  }
  free(block.table);
  for(int i=n;i<header.last_available - 1;i++)
  {
    header.table[i] = header.table[i+1];
  }
  header.table[header.last_available - 1].table = NULL;
  header.last_available = header.last_available-1;
  return header;
}

void print_block(struct main_table_header header, int n)
{
  if(header.last_available <= n)
  {
    printf("%s\n","INSERTED WRONG INDEX TO PRINT");
    return;
  }
  if(header.table[n].table == NULL)
  {
    return;
  }
  struct rows_table_header block = header.table[n];
  for(int i=0; i<block.rows_n;i++)
  {
    if(block.table[i] != NULL)
      printf("%s",block.table[i]);
  }
  printf("\n");
}

void free_memory(struct main_table_header header)
{
  for(int i = 0; i<header.last_available; i++)
  {
    struct rows_table_header rows_table = header.table[i];
    if(rows_table.table != NULL)
    {
      for(int j = 0; j<rows_table.rows_n; j++)
      {
        free(rows_table.table[j]);
      }
    }
    free(rows_table.table);
  }
  free(header.table);
}

struct files_list_header free_list(struct files_list_header header)
{
  for(int i = 0; i<header.size; i++)
  {
    free(header.files_list[i]);
  }
  free(header.files_list);
  header.size = 0;
  return header;
}
