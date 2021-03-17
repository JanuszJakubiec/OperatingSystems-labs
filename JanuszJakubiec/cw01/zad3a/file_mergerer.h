#ifndef FILE_MERGERER_H
#define FILE_MERGERER_H
#include <stdio.h>

struct rows_table_header {
  int rows_n;
  char** table;
  int deleted;
};

struct main_table_header {
  int blocks_n;
  int last_available;
  struct rows_table_header* table;
  int max_available;
};

struct files_list_header {
  int size;
  char** files_list;
};

struct tmp_files_header {
  int size;
  FILE** table;
};

struct main_table_header create_table(int n);
void print_block(struct main_table_header header, int n);
void free_memory(struct main_table_header header);
struct main_table_header delete_block(struct main_table_header header, int block_n);
FILE* merge_two_files(char file1[100], char file2[100]);
struct tmp_files_header merge_many_files(struct files_list_header files, struct tmp_files_header tmp_files);
struct rows_table_header save_single_file(FILE * tmp);
struct main_table_header save_tmp_to_table(struct tmp_files_header* files_header, struct main_table_header header);
int get_number_of_rows(struct main_table_header header, int block_n);
struct tmp_files_header delete_tmp_files(struct tmp_files_header header);
struct files_list_header free_list(struct files_list_header header);
struct main_table_header delete_row_from_table(struct main_table_header header, int block_n, int row_n);

#endif
