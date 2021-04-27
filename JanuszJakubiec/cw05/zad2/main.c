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

int main(int argc, char *argv[])
{
  if(argc != 2 && argc != 4)
  {
    printf("%s\n", "Wrong number of arguments inserted");
    return 0;
  }
  if(argc == 2)
  {
    if(strcmp(argv[1],"data") == 0)
    {
      FILE* mail_reader =  popen("mail | sort -k5,5 -k6,6 -k7,7", "w");
      fputs("exit", mail_reader);
      pclose(mail_reader);
    }
    if(strcmp(argv[1], "nadawca") == 0)
    {
      FILE* mail_reader =  popen("mail | sort -k3,3", "w");
      fputs("exit", mail_reader);
      pclose(mail_reader);
    }
  }
  if(argc == 4)
  {
    char message[100] = "mail ";
    strcat(message, argv[1]);
    strcat(message, "@localhost ");
    strcat(message, "-s ");
    strcat(message, argv[2]);
    FILE* mail_sender =  popen(message, "w");
    fputs(argv[3], mail_sender);
    pclose(mail_sender);
  }
}
