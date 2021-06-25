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
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>

//server msg types
#define REGISTER_CUSTOMER 1
#define FIND_MATE 2
#define MAKE_MOVE 3
#define CLIENT_DISCONNECTED 4
#define ALIVE_INFO 5

//client msg types
#define CLIENT_REGISTERED 1
#define SERVER_FULL 2
#define NAME_RESERVED 3
#define GAME_CREATED 4
#define GAME_ENDED 5
#define NAME_DOESNT_EXIST 6
#define AM_I_ALIVE 7
#define MOVE_ILLEGAL 8
#define PARTNER_BUSY 9
#define MY_MOVE 10
#define OPPONENT_MOVE 11
#define OPPONENT_DISCONNECTED 12

#define NAME_MAX_LEN 30

struct msg_to_server
{
  int msg_type;
  int id;
  char name[NAME_MAX_LEN];
  int move;
};

struct msg_to_client
{
  int msg_type;
  int number;
  char board[9];
};
