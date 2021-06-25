#include "header.h"

#define UNIX_PATH_MAX 108
#define CLIENTS_N 100
#define BACKLOG 101

struct client
{
  char* name;
  int fd;
  int is_active;
  int is_playing_the_game;
  struct epoll_event* event;
  pthread_t game;
  int candidate_to_be_deleted;
};

struct msg_to_server move_message;
int is_message_new;

struct client clients_list[CLIENTS_N];
int clients_n;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t message_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t message_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t read_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t epoll_mutex = PTHREAD_MUTEX_INITIALIZER;
int epoll;

int game_has_been_won(char* tab)
{
  if(tab[0] == tab[1] && tab[1] == tab[2] && tab[0] != ' ')
    return 2;
  if(tab[3] == tab[4] && tab[4] == tab[5] && tab[3] != ' ')
    return 2;
  if(tab[6] == tab[7] && tab[7] == tab[8] && tab[6] != ' ')
    return 2;
  if(tab[0] == tab[3] && tab[3] == tab[6] && tab[0] != ' ')
    return 2;
  if(tab[1] == tab[4] && tab[4] == tab[7] && tab[1] != ' ')
    return 2;
  if(tab[2] == tab[5] && tab[5] == tab[8] && tab[2] != ' ')
    return 2;
  if(tab[0] == tab[4] && tab[4] == tab[8] && tab[0] != ' ')
    return 2;
  if(tab[2] == tab[4] && tab[4] == tab[6] && tab[6] != ' ')
    return 2;
  int free_space = 0;
  for(int i =0; i<9;i++)
  {
    if(tab[i] == ' ')
      free_space++;
  }
  if(free_space == 0)
    return 1;
  return -1;
}

struct msg_to_client copy_board(char * board)
{
  struct msg_to_client msg;
  for(int i = 0;i <9; i++)
  {
    msg.board[i] = board[i];
  }
  return msg;
}

void msg_cpy(struct msg_to_server* m1, struct msg_to_server* m2)
{
  m2->msg_type = m1->msg_type;
  m2->id = m1->id;
  m2->move = m1->move;
  strcpy(m2->name, m1->name);
}

void send_msg_to_client(int fd, struct msg_to_client msg_response)
{
  write(fd, &msg_response, sizeof(struct msg_to_client));
}

void* game(void* arg)
{
  struct msg_to_server d = *((struct msg_to_server *) (arg));
  struct msg_to_server data;
  msg_cpy(&d, &data);
  free(arg);
  struct msg_to_client msg_response;
  char board[9];
  for(int i = 0; i<9; i++)
  {
    board[i] = ' ';
  }

  pthread_mutex_lock(&mutex);
  int id1 = data.id;
  int desc1 = clients_list[id1].fd;
  int id2 = -1;
  int desc2;
  for(int i = 0; i<CLIENTS_N; i++)
  {
    if(clients_list[i].is_active == 1 && strcmp(clients_list[i].name, data.name) == 0 && i != id1)
    {
      if(clients_list[i].is_playing_the_game != -1)
      {
         msg_response.msg_type = PARTNER_BUSY;
         send_msg_to_client(desc1, msg_response);
         pthread_mutex_unlock(&mutex);
         return NULL;
      }
      id2 = i;
      desc2 = clients_list[i].fd;
      clients_list[i].is_playing_the_game = id1;
      break;
    }
  }
  pthread_mutex_unlock(&mutex);
  if(id2 == -1 && id2 != id1)
  {
    msg_response.msg_type = NAME_DOESNT_EXIST;
    send_msg_to_client(desc1, msg_response);
    return NULL;
  }
  else
  {
    pthread_mutex_lock(&mutex);
      clients_list[id1].is_playing_the_game = id2;
      clients_list[id2].is_playing_the_game = id1;
    pthread_mutex_unlock(&mutex);
    struct msg_to_client msg_response = copy_board(board);
    msg_response.msg_type = GAME_CREATED;
    msg_response.number = 1;
    send_msg_to_client(desc1, msg_response);
    msg_response.number = 0;
    send_msg_to_client(desc2, msg_response);
  }
  while(1)
  {
    struct msg_to_server msg;
    pthread_mutex_lock(&message_mutex);
    while(!(is_message_new == 1 && (move_message.id == id1 || move_message.id == id2)))
    {
      pthread_cond_wait(&message_cond, &message_mutex);
    }
    msg_cpy(&move_message, &msg);
    is_message_new = 0;
    pthread_cond_broadcast(&read_cond);
    pthread_mutex_unlock(&message_mutex);
    if(msg.move == -10)
    {
      struct msg_to_client msg_response = copy_board(board);
      msg_response.msg_type = OPPONENT_DISCONNECTED;
      pthread_mutex_lock(&mutex);
      if(msg.id == id1 && clients_list[id2].is_active == 1)
      {
        send_msg_to_client(desc2, msg_response);
        clients_list[id2].is_playing_the_game = -1;
      }
      if(msg.id == id2 && clients_list[id1].is_active == 1)
      {
        send_msg_to_client(desc1, msg_response);
        clients_list[id1].is_playing_the_game = -1;
      }
      printf("%s\n", "GAME ENDED");
      pthread_mutex_unlock(&mutex);
      return NULL;
    }
    if(msg.move < 1 || msg.move > 9 || board[msg.move-1] != ' ')
    {
      printf("move: %d\n", msg.move);
      msg_response.msg_type = MOVE_ILLEGAL;
      if(msg.id == id1)
        send_msg_to_client(desc1, msg_response);
      else
        send_msg_to_client(desc2, msg_response);
    }
    else
    {
      if(msg.id == id1)
      {
        board[msg.move-1] = 'x';
        int num = game_has_been_won(board);
        if(num>0)
        {
          struct msg_to_client msg_response = copy_board(board);
          msg_response.msg_type = GAME_ENDED;
          msg_response.number = num;
          send_msg_to_client(desc1, msg_response);
          msg_response.msg_type = GAME_ENDED;
          msg_response.number = -num;
          send_msg_to_client(desc2, msg_response);
          pthread_mutex_lock(&mutex);
          {
            clients_list[id1].is_playing_the_game = -1;
            clients_list[id2].is_playing_the_game = -1;
          }
          pthread_mutex_unlock(&mutex);
          return NULL;
        }
        else
        {
          struct msg_to_client msg_response = copy_board(board);
          msg_response.msg_type = MY_MOVE;
          send_msg_to_client(desc1, msg_response);
          msg_response.msg_type = OPPONENT_MOVE;
          send_msg_to_client(desc2, msg_response);
        }
      }
      else
      {
        board[msg.move-1] = 'o';
        int num = game_has_been_won(board);
        if(num > 0)
        {
          struct msg_to_client msg_response = copy_board(board);
          msg_response.msg_type = GAME_ENDED;
          msg_response.number = num;
          send_msg_to_client(desc2, msg_response);
          msg_response.msg_type = GAME_ENDED;
          msg_response.number = -num;
          send_msg_to_client(desc1, msg_response);
          pthread_mutex_lock(&mutex);
          {
            clients_list[id1].is_playing_the_game = -1;
            clients_list[id2].is_playing_the_game = -1;
          }
          pthread_mutex_unlock(&mutex);
          return NULL;
        }
        else
        {
          struct msg_to_client msg_response = copy_board(board);
          msg_response.msg_type = MY_MOVE;
          send_msg_to_client(desc2, msg_response);
          msg_response.msg_type = OPPONENT_MOVE;
          send_msg_to_client(desc1, msg_response);
        }
      }
    }
  }
}

void delete_client(int index)
{
  int is_playing_the_game;
  pthread_mutex_lock(&mutex);
  int fd = clients_list[index].fd;
  struct epoll_event* socket_event = clients_list[index].event;
  clients_list[index].is_active = 0;
  is_playing_the_game = clients_list[index].is_playing_the_game;
  pthread_mutex_unlock(&mutex);
  epoll_ctl(epoll, EPOLL_CTL_DEL, fd, socket_event);
  free(socket_event);
  if(is_playing_the_game == -1)
    return;
  pthread_mutex_lock(&message_mutex);
  while(is_message_new == 1)
  {
    pthread_cond_wait(&read_cond, &message_mutex);
  }
  move_message.move = -10;
  move_message.id = index;
  is_message_new = 1;
  pthread_cond_broadcast(&message_cond);
  pthread_mutex_unlock(&message_mutex);
  return;
}

int create_local_socket(char* path)
{
  int addr = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un socket_struct;
  memset(&socket_struct, 0, sizeof(struct sockaddr_un));
  socket_struct.sun_family = AF_UNIX;
  strcpy(socket_struct.sun_path, path);
  unlink(path);
  bind(addr, (struct sockaddr*)&socket_struct, sizeof(struct sockaddr_un));
  listen(addr, BACKLOG);
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
  socket_struct.sin_addr.s_addr = INADDR_ANY;
  bind(addr, (struct sockaddr*)&socket_struct, sizeof(struct sockaddr_in));
  listen(addr, BACKLOG);
  return addr;
}

void* check_clients(void* arg)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    struct msg_to_client msg;
    msg.msg_type = AM_I_ALIVE;
    for(int i = 0; i<CLIENTS_N; i++)
    {
      if(clients_list[i].is_active == 1)
      {
        clients_list[i].candidate_to_be_deleted = 1;
        send_msg_to_client(clients_list[i].fd, msg);
      }
    }
    pthread_mutex_unlock(&mutex);
    sleep(5);
    for(int i = 0; i<CLIENTS_N; i++)
    {
      int flag = 0;
      pthread_mutex_lock(&mutex);
      if(clients_list[i].is_active == 1 && clients_list[i].candidate_to_be_deleted == 1)
      {
        flag = 1;
      }
      pthread_mutex_unlock(&mutex);
      if(flag)
        delete_client(i);
    }
  }
}

int add_client(struct msg_to_server msg, int cli_desc)
{
  int index = -1;
  for(int i = 0; i < CLIENTS_N; i++)
  {
    if(clients_list[i].is_active == 0)
    {
      if(index == -1)
        index = i;
    }
    else
    {
      if(strcmp(clients_list[i].name, msg.name) == 0)
      {
        struct msg_to_client msg_response;
        msg_response.msg_type = NAME_RESERVED;
        send_msg_to_client(cli_desc, msg_response);
        return -1;
      }
    }
  }
  if(index == -1)
  {
    struct msg_to_client msg_response;
    msg_response.msg_type = SERVER_FULL;
    send_msg_to_client(cli_desc, msg_response);
    return -1;
  }

  pthread_mutex_lock(&mutex);
  clients_list[index].name = calloc(strlen(msg.name), sizeof(char));
  strcpy(clients_list[index].name, msg.name);
  clients_list[index].fd = cli_desc;
  clients_list[index].is_active = 1;
  clients_list[index].is_playing_the_game = -1;
  clients_list[index].event = calloc(1, sizeof(struct epoll_event));
  clients_list[index].event->events = EPOLLIN|EPOLLRDHUP;
  clients_list[index].event->data.fd = cli_desc;
  pthread_mutex_unlock(&mutex);
  struct msg_to_client msg_response;
  msg_response.msg_type = CLIENT_REGISTERED;
  msg_response.number = index;
  send_msg_to_client(cli_desc, msg_response);
  clients_n++;
  return index;
}

int main(int argc, char *argv[])
{
  if(argc < 3)
  {
    printf("%s\n", "Zbyt malo argumentow");
    return 0;
  }
  clients_n = 0;
  is_message_new = 0;

  for(int i = 0; i < CLIENTS_N; i++)
  {
    clients_list[i].is_active = 0;
  }

  char* port = argv[1];
  char* path = argv[2];
  int local_socket = create_local_socket(path);
  int global_socket = create_global_socket(port);
  pthread_t client_checker;
  int customer_descriptor;
  epoll = epoll_create1(0);
  struct epoll_event event1, event2;
  event1.events = EPOLLIN;
  event1.data.fd = local_socket;
  event2.events = EPOLLIN;
  event2.data.fd = global_socket;
  epoll_ctl(epoll, EPOLL_CTL_ADD, local_socket, &event1);
  epoll_ctl(epoll, EPOLL_CTL_ADD, global_socket, &event2);

  pthread_create(&client_checker, NULL, check_clients, NULL);
  while(1)
  {
    struct epoll_event result_event;
    epoll_wait(epoll, &result_event, 1, -1);
    if(result_event.data.fd == local_socket || result_event.data.fd == global_socket)
    {
      customer_descriptor = accept(result_event.data.fd, NULL, NULL);
      result_event.data.fd = customer_descriptor;
    }
    struct msg_to_server* msg = calloc(1, sizeof(struct msg_to_server));
    int n = read(result_event.data.fd, msg, sizeof(struct msg_to_server));
    if(n == 0)
    {
      pthread_mutex_lock(&mutex);
      int i = 0;
      for(i = 0; i<CLIENTS_N;i++)
      {
        if(clients_list[i].is_active == 1 && clients_list[i].fd == result_event.data.fd)
        {
          break;
        }
      }
      pthread_mutex_unlock(&mutex);
      if(i < CLIENTS_N)
      {
        delete_client(i);
      }
      continue;
    }
    if(msg->msg_type == REGISTER_CUSTOMER)
    {
      int index = add_client(*msg, customer_descriptor);
      if(index != -1)
      {
        pthread_mutex_lock(&mutex);
        epoll_ctl(epoll, EPOLL_CTL_ADD, clients_list[index].fd, clients_list[index].event);
        pthread_mutex_unlock(&mutex);
      }
    }
    if(msg->msg_type == FIND_MATE)
    {
      pthread_mutex_lock(&mutex);
      pthread_create(&clients_list[msg->id].game, NULL, game, (void*)msg);
      pthread_mutex_unlock(&mutex);
    }
    if(msg->msg_type == MAKE_MOVE)
    {
      pthread_mutex_lock(&message_mutex);
      while(is_message_new == 1)
      {
        pthread_cond_wait(&read_cond, &message_mutex);
      }
      msg_cpy(msg, &move_message);
      is_message_new = 1;
      pthread_cond_broadcast(&message_cond);
      free(msg);
      pthread_mutex_unlock(&message_mutex);
    }
    if(msg->msg_type == CLIENT_DISCONNECTED)
    {
      delete_client(msg->id);
      free(msg);
    }
    if(msg->msg_type == ALIVE_INFO)
    {
      pthread_mutex_lock(&mutex);
      clients_list[msg->id].candidate_to_be_deleted = 0;
      pthread_mutex_unlock(&mutex);
      free(msg);
    }
  }
}
