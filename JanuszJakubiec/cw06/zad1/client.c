#include "header.h"
pid_t my_child_pid;
int my_queue, server_queue, my_key, my_id, partner_queue;

void sig_int_handler_child(int sig)
{
  struct msg_to_server msg;
  msg.mtype = STOP_TO_SRV;
  msg.my_key = my_key;
  msg.my_id = my_id;
  msgsnd(server_queue, (void*)&msg, sizeof(struct msg_to_server) - sizeof(long), 0);
  msgctl(my_queue, IPC_RMID, NULL);
  exit(0);
}

void sig_int_handler_parent(int sig)
{
  kill(my_child_pid, SIGINT);
  exit(0);
}

int init_server_comunication()
{
  struct msg_to_server msg;
  msg.mtype = INIT;
  msg.my_key = my_key;
  if(msgsnd(server_queue, (void*)&msg, sizeof(struct msg_to_server) - sizeof(long), 0) != 0)
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
    msgsnd(partner_queue, (void*)&msg_to_p, sizeof(struct msg_to_client) - sizeof(long), 0);
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
      msgsnd(server_queue, (void*)&msg, sizeof(struct msg_to_server) - sizeof(long), 0);
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
      msgsnd(server_queue, (void*)&msg, sizeof(struct msg_to_server) - sizeof(long), 0);
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
        msgsnd(server_queue, (void*)&msg, sizeof(struct msg_to_server) - sizeof(long), 0);
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
  int server_key, partner_key;
  server_key = ftok(HOME, 's');
  server_queue = msgget(server_key, IPC_EXCL);
  init_server_comunication();
  struct msg_to_client receive;
  while(1)
  {
    if(msgrcv(my_queue, (void*)&receive, sizeof(struct msg_to_client) - sizeof(long), -20, 0)>0)
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
        int tmp_key = receive.new_chat_key;
        if(tmp_key < 0)
        {
          if(tmp_key == -2)
          {
            printf("%s\n","Client with which you want to establish connection is busy");
          }
          else
          {
            printf("%s\n","Client with which you want to establish connection does not exist");
          }
        }
        else
        {
          if(tmp_key == 0)
          {
            printf("%s\n", "Connection closed");
            partner_key = -1;
            partner_queue = -1;
          }
          else
          {
            printf("%s\n", "Connected successfully");
            partner_key = tmp_key;
            partner_queue = msgget(partner_key, IPC_EXCL);
          }
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  partner_queue = -1;
  my_key = ftok(HOME, 'c');
  my_queue = msgget(my_key, 0777| IPC_CREAT | IPC_EXCL);
  while(my_queue == -1)
  {
    my_key = my_key+1;
    my_queue = msgget(my_key, 0777| IPC_CREAT | IPC_EXCL);
  }
  my_child_pid = fork();
  if(my_child_pid == 0)
  {
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
      msgsnd(my_queue, (void*)&msg, sizeof(struct msg_to_client) - sizeof(long), 0);
    }
    for(int i = 0; i < MSG_SIZE; i++)
    {
      msg.tab[i] = '\0';
    }
  }
  return 1;
}
