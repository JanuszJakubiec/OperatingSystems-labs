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
#include <sys/msg.h>

#define MSG_SIZE 4000
#define HOME getenv("HOME")
#define STOP_CLI 1
#define CONNECT_CLI 2
#define INIT_CLI 3
#define LIST_CLI 4
#define CHAT_CLI 5
#define INNER_CLI 10

#define STOP_TO_SRV 1
#define DISCONNECT_CHAT 2
#define LIST_CLIENTS 3
#define CONNECT_TO 4
#define INIT 5

#define SIZE_IN_HEAD connect

struct msg_to_server
{
  long mtype; // 1-> STOP, 2->Disconnect, 3->LIST, 4->Connect, 5->INIT
  int my_key;
  int my_id;
  int connect_to;
};

struct msg_to_client
{
  long mtype; // 1 -> disconnected, 2 -> set_chat_mode, 3 -> init_message, 4-> return list, 5->chat
  int your_id;
  int new_chat_key;
  char tab[MSG_SIZE];
};
