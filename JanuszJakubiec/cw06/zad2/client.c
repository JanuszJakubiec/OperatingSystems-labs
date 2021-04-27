#include "header.h"
pid_t my_child_pid;
mqd_t my_queue, server_queue, partner_queue;
char my_key[KEY_LEN];
int my_id;

void sig_int_handler_child(int sig)
{
  if(partner_queue >= 0)
  {
    mq_close(partner_queue);
  }
  struct msg_to_server msg;
  msg.mtype = STOP_TO_SRV;
  msg.my_id = my_id;
  mq_send(server_queue, (char *)&msg, sizeof(struct msg_to_server), STOP_TO_SRV);
  mq_close(server_queue);
  mq_close(my_queue);
  printf("%d\n", mq_unlink(my_key));
  exit(0);
}

void sig_int_handler_parent(int sig)
{
  mq_close(my_queue);
  kill(my_child_pid, SIGINT);
  exit(0);
}

int init_server_comunication()
{
  struct msg_to_server msg;
  msg.mtype = INIT;
  strcpy(msg.key,my_key);
  if(mq_send(server_queue, (char*)&msg, sizeof(struct msg_to_server), INIT)!=0)
  {
    printf("%s\n", "I was unable to send a message!");
    kill(getppid(), SIGINT);
  }
  return 0;
}

void process_command(char* string)
{
  if(strlen(string) <2)
  {
    printf("%s\n", "INCORRECT COMMAND! TOO SHORT!");
    return;
  }
  struct msg_to_server msg;
  msg.my_id = my_id;
  if(string[0] == 'M' && string[1] == ' ')
  {
    if(partner_queue < 0)
    {
      printf("%s\n", "There is no connection!!");
      return;
    }
    struct msg_to_client msg_to_p;
    for(int i = 2; i<MSG_SIZE; i++)
    {
      msg_to_p.mtype = CHAT_CLI;
      msg_to_p.tab[i-2] = string[i];
    }
    mq_send(partner_queue, (char*)&msg_to_p, sizeof(struct msg_to_client), CHAT_CLI);
  }
  else
  {
    char* tmp = strtok_r(string, " ", &string);
    int flag = 0;
    if(strcmp(tmp, "STOP") == 0)
    {
      flag = 1;
      printf("%s\n","Stopping programm");
      kill(getppid(), SIGINT);
    }
    if(strcmp(tmp, "LIST") == 0)
    {
      flag = 1;
      msg.mtype = LIST_CLIENTS;
      mq_send(server_queue, (char*)&msg, sizeof(struct msg_to_server), LIST_CLIENTS);
    }
    if(strcmp(tmp, "DISCONNECT") == 0)
    {
      if(partner_queue == -1)
      {
        printf("I am not connected\n");
        return;
      }
      flag = 1;
      msg.mtype = DISCONNECT_CHAT;
      mq_send(server_queue, (char *)&msg, sizeof(struct msg_to_server), DISCONNECT_CHAT);
    }
    if(strcmp(tmp, "CONNECT") == 0)
    {
      flag = 1;
      if(partner_queue != -1)
      {
        printf("I am already connected\n");
      }
      else
      {
        msg.connect_to = atoi(string);
        msg.mtype = CONNECT_TO;
        mq_send(server_queue, (char *)&msg, sizeof(struct msg_to_server), CONNECT_TO);
      }
    }
    if(flag == 0)
    {
      printf("Can't understand command\n");
    }
  }
}

void commmunicate()
{
  char buffer[MSG_SIZE];
  char server_key[KEY_LEN] = "/server";
  server_queue = mq_open(server_key, O_WRONLY);
  init_server_comunication();
  struct msg_to_client receive;
  while(1)
  {
    if(mq_receive(my_queue, (char*)&receive, sizeof(struct msg_to_client), NULL)>0)
    {
      if(receive.mtype == STOP_CLI)
      {
        printf("%s\n", "Server stopped");
        kill(getppid(), SIGINT);
      }
      if(receive.mtype == INIT_CLI)
      {
        my_id = receive.your_id;
      }
      if(receive.mtype == LIST_CLI)
      {
        printf("%s", receive.tab);
      }
      if(receive.mtype == CHAT_CLI)
      {
        printf("%s\n", receive.tab);
      }
      if(receive.mtype == INNER_CLI)
      {
        strcpy(buffer, receive.tab);
        process_command(buffer);
      }
      if(receive.mtype == CONNECT_CLI)
      {
        if(receive.key[0] == 'a')
        {
          printf("%s\n","Client with which you want to establish connection is busy");
        }
        if(receive.key[0] == 'b')
        {
          printf("%s\n","Client with which you want to establish connection does not exist");
        }
        if(receive.key[0] == 'c')
        {
          printf("%s\n", "Connection closed");
          mq_close(partner_queue);
          partner_queue = -1;
        }
        if(receive.key[0] == '/')
        {
          printf("%s\n", "Connected successfully");
          partner_queue = mq_open(receive.key, O_WRONLY);
        }
      }
    }
  }
}

void generate_key(char* key)
{
  static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
  key[0] = '/';
  for(int i = 1; i<KEY_LEN-1; i++)
  {
    int num = rand() % (int) (sizeof charset - 1);
    key[i] = charset[num];
  }
  key[KEY_LEN - 1] = '\0';
}

int main(int argc, char *argv[])
{
  struct mq_attr attr1;
  attr1.mq_msgsize = sizeof(struct msg_to_client);
  attr1.mq_maxmsg = 10;
  srand(time(NULL));
  partner_queue = -1;
  my_queue = -1;
  while(my_queue == -1) {
    generate_key(my_key);
    my_queue = mq_open(my_key, O_CREAT|O_WRONLY, 0777, &attr1);
  }
  my_child_pid = fork();
  if(my_child_pid == 0)
  {
    mq_close(my_queue);
    my_queue = mq_open(my_key, O_RDONLY);
    struct sigaction act1;
    sigemptyset(&act1.sa_mask);
    act1.sa_flags = 0;
    act1.sa_handler = sig_int_handler_child;
    sigaction(SIGINT, &act1, NULL);
    commmunicate();
    exit(0);
  }
  FILE* in = stdin;
  struct sigaction act1;
  sigemptyset(&act1.sa_mask);
  act1.sa_flags = 0;
  act1.sa_handler = sig_int_handler_parent;
  sigaction(SIGINT, &act1, NULL);
  int counter = 0;
  while(1)
  {
    struct msg_to_client msg;
    msg.mtype = INNER_CLI;
    char sign = '\0';
    counter = 0;
    while(sign != '\n' && counter < MSG_SIZE-1)
    {
      if(fread(&sign, sizeof(char), sizeof(char), in)==0)
      {
        break;
      }
      msg.tab[counter] = sign;
      counter++;
    }
    msg.tab[counter-1] = '\0';
    if(counter == MSG_SIZE-1)
    {
      printf("%s: %d signs\n", "Your message was too long, please ensure, your message is not longer than", MSG_SIZE-1);
    }
    else
    {
      mq_send(my_queue, (char *)&msg, sizeof(struct msg_to_client), INNER_CLI);
    }
    for(int i = 0; i < MSG_SIZE; i++)
    {
      msg.tab[i] = '\0';
    }
  }
  return 1;
}
