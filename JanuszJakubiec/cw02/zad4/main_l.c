#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>

void find_and_replace(char* buffer, char* n1, char* n2, FILE* file)
{
  int buffer_l = strlen(buffer);
  int n1_len = strlen(n1);
  int n2_len = strlen(n2);
  for(int i = 0; i<buffer_l; i++)
  {
    if(buffer[i] == n1[0])
    {
      int j = 1;
      for(j = 1; j<n1_len;j++)
      {
        if(buffer[i+j] != n1[j] || i+j >= buffer_l)
          break;
      }
      if(j == n1_len)
      {
        i += j-1;
        for(j = 0; j<n2_len;j++)
        {
          fwrite(&(n2[j]), sizeof(char), sizeof(char), file);
        }
      }
      else
      {
        fwrite(&(buffer[i]), sizeof(char), sizeof(char), file);
      }
    }
    else
    {
      fwrite(&(buffer[i]), sizeof(char), sizeof(char), file);
    }
  }
}

void function(char* file_r, char* file_w, char* n1, char* n2)
{
  char* buffer;
  char sign[0];
  FILE* out = stdout;
  FILE* f_r = fopen(file_r, "r");
  FILE* f_w = fopen(file_w, "w");
  if(f_r == NULL)
  {
    char err1[40] = "file not found\n";
    fwrite(err1, sizeof(char), sizeof(char)*strlen(err1), out);
    return;
  }
  int len;
  fseek(f_r, 0, SEEK_END);
  fgetpos(f_r, &len);
  rewind(f_r);
  printf("%d\n", len);
  buffer = (char*)calloc(len+1, sizeof(char));
  for(int i = 0; i < len; i++)
  {
    fread(sign, sizeof(char), sizeof(char), f_r);
    buffer[i]=sign[0];
  }
  find_and_replace(buffer, n1, n2, f_w);
  fclose(f_r);
  fclose(f_w);
  free(buffer);
}
int main()
{
  function("a", "b", "b\naa", "gg");
}
