#include "header.h"

#define UNIX_PATH_MAX 108
#define BACKLOG 101

int customer_number = -1;
int server_desc;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mutex_cond = PTHREAD_COND_INITIALIZER;
char shared_buff[NAME_MAX_LEN];
int shared_buff_free;
int amIPlayingAGame;
int table[9];
int amIcross;
int amIwaitingForMove;

void quit()
{
  struct msg_to_server msg;
  msg.msg_type = CLIENT_DISCONNECTED;
  msg.id = customer_number;
  write(server_desc, &msg, sizeof(struct msg_to_server));
  exit(0);
}

void display_board(struct  msg_to_client msg)
{
  char number = '1';
  for(int i = 0;i<9;i++)
  {
    if(msg.board[i] == ' ')
      msg.board[i] = number;
    number ++;
  }
  printf("%c|%c|%c\n", msg.board[0], msg.board[1], msg.board[2]);
  printf("-----\n");
  printf("%c|%c|%c\n", msg.board[3], msg.board[4], msg.board[5]);
  printf("-----\n");
  printf("%c|%c|%c\n", msg.board[6], msg.board[7], msg.board[8]);
}

void* server_communicator_func()
{
  while(1)
  {
    struct msg_to_client msg;
    if(recv(server_desc, &msg, sizeof(struct msg_to_client), MSG_DONTWAIT) > 0)
    {
      if(msg.msg_type == GAME_CREATED)
      {
        amIPlayingAGame = 1;
        amIcross = msg.number;
        if(amIcross == 0)
          amIwaitingForMove = 1;
        else
          amIwaitingForMove = 0;
        display_board(msg);
      }
      if(msg.msg_type == OPPONENT_MOVE)
      {
        amIwaitingForMove = 0;
        display_board(msg);
      }
      if(msg.msg_type == OPPONENT_DISCONNECTED)
      {
        amIPlayingAGame = 0;
        printf("%s\n", "I WON");
        amIwaitingForMove = 0;
      }
      if(msg.msg_type == MY_MOVE)
      {
        display_board(msg);
      }
      if(msg.msg_type == GAME_ENDED)
      {
        display_board(msg);
        amIPlayingAGame = 0;
        if(msg.number == -2)
          printf("%s\n", "I LOST");
        if(msg.number == 1 || msg.number == -1)
          printf("%s\n", "DRAW");
        if(msg.number == 2)
          printf("%s\n", "I WON");
        amIwaitingForMove = 0;
      }
      if(msg.msg_type == NAME_DOESNT_EXIST)
      {
        printf("%s\n", "USER WITH THAT USERNAME DOES NOT EXIST");
      }
      if(msg.msg_type == MOVE_ILLEGAL)
      {
        printf("%s\n", "MOVE INSERTED WAS ILLEGAL");
        amIwaitingForMove = 0;
      }
      if(msg.msg_type == PARTNER_BUSY)
      {
        printf("%s\n", "Partner is busy");
        amIwaitingForMove = 0;
        amIPlayingAGame = 0;
      }
      if(msg.msg_type == AM_I_ALIVE)
      {
        struct msg_to_server msg;
        msg.msg_type = ALIVE_INFO;
        msg.id = customer_number;
        write(server_desc, &msg, sizeof(struct msg_to_server));
      }
    }
    pthread_mutex_lock(&mutex);
    if(shared_buff_free == 0)
    {
      if(amIPlayingAGame == 0)
      {
        struct msg_to_server msg;
        msg.msg_type = FIND_MATE;
        msg.id = customer_number;
        strcpy(msg.name, shared_buff);
        shared_buff_free = 1;
        write(server_desc, &msg, sizeof(struct msg_to_server));
      }
      if(amIPlayingAGame == 1)
      {
        if(amIwaitingForMove == 0)
        {
          struct msg_to_server msg;
          msg.msg_type = MAKE_MOVE;
          msg.id = customer_number;
          shared_buff[1] = '\0';
          msg.move = atoi(shared_buff);
          shared_buff_free = 1;
          amIwaitingForMove = 1;
          write(server_desc, &msg, sizeof(struct msg_to_server));
        }
        else
        {
          printf("%s\n", "The move was made, you must wait for the response before you make another move");
          shared_buff_free = 1;
        }
      }
    }
    pthread_mutex_unlock(&mutex);
    usleep(10000);
  }
}

int create_local_socket(char* path)
{
  int addr = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un socket_struct;
  memset(&socket_struct, 0, sizeof(struct sockaddr_un));
  socket_struct.sun_family = AF_UNIX;
  strcpy(socket_struct.sun_path, path);
  connect(addr, (struct sockaddr*)&socket_struct, sizeof(struct sockaddr_un));

  return addr;
}

int create_global_socket(char *port)
{
  int p = atoi(port);
  short short_port = (short)p;
  int addr = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in socket_struct;
  memset(&socket_struct, 0, sizeof(struct sockaddr_in));
  socket_struct.sin_family = AF_INET;
  socket_struct.sin_port = htons(short_port);
  inet_pton(AF_INET, "127.0.0.1", &socket_struct.sin_addr);
  connect(addr, (struct sockaddr*)&socket_struct, sizeof(struct sockaddr_in));
  return addr;
}

int main(int argc, char *argv[])
{
  shared_buff_free = 1;
  amIPlayingAGame = 0;
  if(argc < 4)
  {
    printf("%s\n", "Zbyt malo argumentow");
    return 0;
  }
  signal(SIGINT, quit);
  struct msg_to_server msg;
  msg.msg_type = REGISTER_CUSTOMER;
  strcpy(msg.name,argv[1]);

  int socket;
  if(strcmp(argv[2], "local") == 0)
  {
    socket = create_local_socket(argv[3]);
  }
  else
  {
    socket = create_global_socket(argv[3]);
  }
  write(socket, &msg, sizeof(struct msg_to_server));
  struct msg_to_client response;
  read(socket, &response, sizeof(struct msg_to_client));
  server_desc = socket;
  if(response.msg_type == SERVER_FULL)
  {
    printf("%s\n", "SERVER IS FULL");
    return 0;
  }
  if(response.msg_type == NAME_RESERVED)
  {
    printf("%s\n", "Client with my name already exists");
    return 0;
  }
  if(response.msg_type != CLIENT_REGISTERED)
  {
    printf("%s\n", "UNKNOWN ERROR OCCURED");
    return 0;
  }
  customer_number = response.number;
  pthread_t server_communicator;
  pthread_create(&server_communicator, NULL, server_communicator_func, NULL);
  char buffer[NAME_MAX_LEN];
  char sign = '\0';
  int counter = 0;
  while(1)
  {
    while(sign != '\n')
    {
      read(0, &sign, sizeof(char));
      buffer[counter] = sign;
      counter++;
      if(counter == NAME_MAX_LEN)
      {
        printf("%s\n", "INPUT TEXT TOO LONG!");
        break;
      }
    }
    sign = '\0';
    buffer[counter-1] = '\0';
    counter = 0;
    printf("%s\n", buffer);
    pthread_mutex_lock(&mutex);
    while(shared_buff_free == 0)
    {
      pthread_cond_wait(&mutex_cond, &mutex);
    }
    strcpy(shared_buff, buffer);
    shared_buff_free = 0;
    pthread_mutex_unlock(&mutex);
  }
  return 0;
}
