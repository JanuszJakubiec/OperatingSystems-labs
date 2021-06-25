#include "header.h"

struct picture source_picture;
struct picture result_picture;

void create_table()
{
  source_picture.pixels = calloc(source_picture.height, sizeof(int*));
  result_picture.pixels = calloc(source_picture.height, sizeof(int*));
  for(int i = 0; i< source_picture.height; i++)
  {
    source_picture.pixels[i] = calloc(source_picture.width, sizeof(int));
    result_picture.pixels[i] = calloc(source_picture.width, sizeof(int));
  }
}

void print_tab()
{
  for(int i = 0; i<source_picture.height; i++)
  {
    for(int j = 0; j<source_picture.width; j++)
    {
      printf("%d ", source_picture.pixels[i][j]);
    }
    printf("\n");
  }
}

void read_picture(int source)
{
  char buffer[10];
  char sign = '0';
  while(sign != '\n')
  {
    read(source, &sign, sizeof(char));
  }
  sign = '\0';
  while(sign != '\n')
  {
    read(source, &sign, sizeof(char));
  }
  int counter = 0;
  int processed = 0;
  while(read(source, &sign, sizeof(char)) != 0)
  {
    if(sign <= '9' && sign >= '0')
    {
      buffer[counter] = sign;
      counter++;
    }
    else
    {
      buffer[counter] = '\0';
      if(counter != 0)
      {
        counter = 0;
        if(processed == 0)
        {
          source_picture.width = atoi(buffer);
          result_picture.width = source_picture.width;
        }
        if(processed == 1)
        {
          source_picture.height = atoi(buffer);
          result_picture.height = source_picture.height;
          create_table();
        }
        if(processed == 2)
        {
          source_picture.M = atoi(buffer);
          result_picture.M = source_picture.M;
        }
        if(processed > 2)
        {
          int w = source_picture.width;
          source_picture.pixels[(processed- 3)/w][(processed - 3)%w] = atoi(buffer);
        }
        counter = 0;
        processed++;
      }
    }
  }
}

void clear_buffer(char* buffer)
{
  for(int i = 0; i<12; i++)
  {
    buffer[i] = '\0';
  }
}


void print_result_file(int file)
{
  char txt_start[25] = "P2\n#Janusz Jakubiec\n";
  char sign = '\0';
  char buffer[12];
  write(file, txt_start, sizeof(char)*strlen(txt_start));

  sprintf(buffer, "%d", result_picture.width);
  write(file, buffer, sizeof(char)*strlen(buffer));
  sign = ' ';
  write(file, &sign, sizeof(char));

  sprintf(buffer, "%d", result_picture.height);
  write(file, buffer, sizeof(char)*strlen(buffer));
  sign = '\n';
  write(file, &sign, sizeof(char));

  sprintf(buffer, "%d", result_picture.M);
  write(file, buffer, sizeof(char)*strlen(buffer));
  sign = '\n';
  write(file, &sign, sizeof(char));

  for(int i = 0; i<result_picture.height; i++)
  {
    sign = ' ';
    for(int j = 0; j<result_picture.width; j++)
    {
      sprintf(buffer, "%d", result_picture.pixels[i][j]);
      write(file, buffer, sizeof(char)*strlen(buffer));
      write(file, &sign, sizeof(char));
    }
    sign = '\n';
    write(file, &sign, sizeof(char));
  }
}

int number_among_my_numbers(int n, int* tab, int size)
{
  for(int i = 0; i<size; i++)
  {
    if(tab[i] == n)
    {
      return 1;
    }
  }
  return 0;
}

void* numbers_transform(void *arg)
{
  struct timeval time_start, time_end;
  gettimeofday(&time_start, NULL);
  long start_time = (time_start.tv_sec * 1000000) + (time_start.tv_usec);
  struct numbers data = *((struct numbers *) (arg));

  int done_operations = 0;
  for(int i = 0; i<source_picture.height; i++)
  {
    for(int j = 0; j<source_picture.width; j++)
    {
      if(number_among_my_numbers(source_picture.pixels[i][j], data.tab, data.size))
      {
        result_picture.pixels[i][j] = source_picture.M - source_picture.pixels[i][j];
        done_operations++;
      }
    }
  }

  free(data.tab);
  long* ret = calloc(1, sizeof(long));
  gettimeofday(&time_end, NULL);
  long end_time = (time_end.tv_sec * 1000000) + (time_end.tv_usec);
  *ret = end_time - start_time;
  pthread_exit((void*) ret);
}

void* block_transform(void *arg)
{
  struct timeval time_start, time_end;
  gettimeofday(&time_start, NULL);
  long start_time = (time_start.tv_sec * 1000000) + (time_start.tv_usec);
  struct block_data data = *((struct block_data *) (arg));

  for(int i = 0; i<source_picture.height; i++)
  {
    for(int j = data.start; j< data.end + 1; j++)
    {
      result_picture.pixels[i][j] = source_picture.M - source_picture.pixels[i][j];
    }
  }

  long* ret = calloc(1, sizeof(long));
  gettimeofday(&time_end, NULL);
  long end_time = (time_end.tv_sec * 1000000) + (time_end.tv_usec);
  *ret = end_time - start_time;
  pthread_exit((void*) ret);
}

int main(int argc, char *argv[])
{
  if(argc < 5)
  {
    printf("%s\n", "not enaugh arguments");
    exit(0);
  }

  int source = open(argv[3], O_RDONLY);
  int result = open(argv[4], O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
  if(source == -1)
  {
    printf("%s\n", "No such file");
    return 0;
  }
  if(result == -1)
  {
    printf("%s\n", "I was not able to create such file");
    return 0;
  }
  int n = atoi(argv[1]);
  pthread_t* threads_tab = calloc(n, sizeof(pthread_t));
  read_picture(source);
  close(source);
  struct timeval time_start, time_end;
  gettimeofday(&time_start, NULL);
  void** data;
  long start_time = (time_start.tv_sec * 1000000) + (time_start.tv_usec);

  if(strcmp(argv[2], "numbers") == 0)
  {
    int numbers_left = source_picture.M + 1;
    struct numbers** numbers_data = calloc(n, sizeof(struct numbers*));
    data = (void**)numbers_data;
    for(int i = 0; i< n; i++)
    {
      numbers_data[i] = calloc(1, sizeof(struct block_data));
      int length = numbers_left/(n-i);
      numbers_data[i]->tab = calloc(length, sizeof(int));
      numbers_data[i]->size = length;
      for(int j = 0; j< length; j++)
      {
        numbers_data[i]->tab[j] = j+source_picture.M - numbers_left+1;
      }
      numbers_left -= length;
      pthread_create(&threads_tab[i], NULL, numbers_transform, (void *) numbers_data[i]);
    }
  }

  else if(strcmp(argv[2], "block") == 0)
  {
    int width_left = source_picture.width;
    struct block_data** block_data = calloc(n, sizeof(struct block_data*));
    data = (void**)block_data;
    for(int i = 0; i< n; i++)
    {
      block_data[i] = calloc(1, sizeof(struct block_data));
      block_data[i]->start = source_picture.width - width_left;
      int length = width_left/(n-i);
      width_left -= length;
      block_data[i]->end = block_data[i]->start + length - 1;
      pthread_create(&threads_tab[i], NULL, block_transform, (void *) block_data[i]);
    }
  }

  else
  {
    return 0;
  }

  long sum = 0;
  for(int i = 0; i< n; i++)
  {
    void* res;
    pthread_join(threads_tab[i], &res);
    long* k = (long*)res;
    sum += *k;
    printf("thread %i time: %ld\n", i, *k);
    free(k);
  }

  gettimeofday(&time_end, NULL);
  long end_time = (time_end.tv_sec * 1000000) + (time_end.tv_usec);
  printf("total time: %ld\n",end_time - start_time);
  printf("summed time, all threads: %ld\n",sum);
  print_result_file(result);
  close(result);
  free(threads_tab);

  for(int i =0; i<n; i++)
  {
    free(data[i]);
  }
  free(data);
  for(int i= 0; i<source_picture.height; i++)
  {
    free(source_picture.pixels[i]);
    free(result_picture.pixels[i]);
  }
  free(source_picture.pixels);
  free(result_picture.pixels);
}
