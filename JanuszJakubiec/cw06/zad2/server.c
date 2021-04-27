#include "header.h"

struct node
{
  int id;
  char key[KEY_LEN];
  int connected;
  struct node* next;
  struct node* prev;
};

struct node* head;
int queue;
char server_key[KEY_LEN];


void INT_block()
{
  sigset_t newmask;
  sigemptyset(&newmask);
  sigaddset(&newmask, SIGINT);
  sigprocmask(SIG_BLOCK, &newmask, NULL);
}

void INT_unblock()
{
  sigset_t newmask;
  sigemptyset(&newmask);
  sigaddset(&newmask, SIGINT);
  sigprocmask(SIG_UNBLOCK, &newmask, NULL);
}

void send_disconnect_message(char* key)
{
  struct msg_to_client msg;
  msg.mtype = CONNECT_CLI;
  msg.key[0] = 'c';
  INT_block();
  mqd_t cli_queue = mq_open(key, O_WRONLY);
  mq_send(cli_queue, (char*)&msg, sizeof(struct msg_to_client), CONNECT_CLI);
  mq_close(cli_queue);
  INT_unblock();
}

struct node* create_list()
{
  struct node* head;
  head = calloc(1, sizeof(struct node));
  head->id = 0;
  head->connected = 0;
  head->next = NULL;
  head->prev = NULL;
  return head;
}

int add_to_list(char* key)
{
  struct node* actual = head;
  for(int i = 0; i<head->connected;i++)
  {
    actual = actual->next;
  }
  struct node* new = calloc(1, sizeof(struct node));
  new->id = actual->id + 1;
  strcpy(new->key, key);
  new->connected = 0;
  new->prev = actual;
  new->next = NULL;
  actual->next = new;
  head->connected++;
  return new->id;
}

struct node* find_by_id(int id)
{
  struct node* actual = head;
  int i = 0;
  for(i = 0; i<head->connected;i++)
  {
    actual = actual->next;
    if(actual->id == id)
      break;
  }
  if(i == head->connected)
    return NULL;
  return actual;
}

int disconnect_clients(int id1)
{
  struct node* c1 = find_by_id(id1);
  if(c1==NULL)
    return -2;
  send_disconnect_message(c1->key);
  if(c1->connected == 0)
  {
    return -1;
  }
  int id2 = c1 -> connected;
  c1->connected = 0;
  struct node* c2 = find_by_id(id2);
  if(c2==NULL)
    return -1;
  c2->connected = 0;
  send_disconnect_message(c2->key);
  return 0;
}

int delete_from_list(int id)
{
  struct node* actual = find_by_id(id);
  if(actual == NULL)
    return -1;
  disconnect_clients(id);
  actual->prev->next = actual->next;
  if(actual->next != NULL)
    actual->next->prev = actual->prev;
  free(actual);
  head->connected -= 1;
  return 0;
}

char* get_key(int id)
{
  struct node* actual = find_by_id(id);
  if(actual == NULL)
    return NULL;
  return actual->key;
}

int connect_clients(int id1, int id2, char* key1, char* key2)
{
  struct node* c1 = find_by_id(id1);
  struct node* c2 = find_by_id(id2);
  if(c1==NULL || c2 == NULL)
    return -1;
  if(c1->connected > 0 || c2->connected > 0)
  {
    return -2;
  }
  c1->connected = c2->id;
  c2->connected = c1->id;
  strcpy(key1,c1->key);
  strcpy(key2,c2->key);
  return 0;
}

void send_init_msg(struct msg_to_server received)
{
  int id = add_to_list(received.key);
  struct msg_to_client msg;
  msg.mtype = INIT_CLI;
  msg.your_id = id;
  INT_block();
  mqd_t cli_queue = mq_open(received.key, O_WRONLY);
  mq_send(cli_queue, (char*)&msg, sizeof(struct msg_to_client), INIT_CLI);
  mq_close(cli_queue);
  INT_unblock();
}

void send_registered_clients_id(struct msg_to_server received)
{
  struct msg_to_client msg;
  msg.mtype = LIST_CLI;
  char num[10];
  char tmp[13];
  for(int i = 0; i<MSG_SIZE; i++)
  {
    msg.tab[i] = '\0';
  }
  int n = head->connected;
  struct node* actual = head;
  for(int i = 0; i<n; i++)
  {
    tmp[0] = '\0';
    actual = actual->next;
    sprintf(num, "%d", actual->id);
    strcat(tmp, num);
    if(actual -> connected == 0)
      strcat(tmp, " 1\n");
    if(actual -> connected != 0)
      strcat(tmp, " 0\n");
    if(strlen(msg.tab) + strlen(tmp) < MSG_SIZE)
    {
      strcat(msg.tab, tmp);
    }
    else
    {
      break;
    }
  }
  struct node* client = find_by_id(received.my_id);
  if(client != NULL)
  {
    INT_block();
    mqd_t cli_queue = mq_open(client->key, O_WRONLY);
    mq_send(cli_queue, (char*)&msg, sizeof(struct msg_to_client), INIT_CLI);
    mq_close(cli_queue);
    INT_unblock();
  }
}

void create_connection(struct msg_to_server received)
{
  char key1[KEY_LEN];
  char key2[KEY_LEN];
  int id1, id2;
  id1 = received.my_id;
  id2 = received.connect_to;
  int res = connect_clients(id1, id2, key1, key2);
  struct msg_to_client msg;
  msg.mtype = CONNECT_CLI;
  if(res == -2)
  {
    msg.key[0] = 'a';
    struct node* client = find_by_id(id1);
    if(client != NULL)
    {
      INT_block();
      mqd_t cli_queue = mq_open(client->key, O_WRONLY);
      mq_send(cli_queue, (char*)&msg, sizeof(struct msg_to_client), CONNECT_CLI);
      mq_close(cli_queue);
      INT_unblock();
    }
    return;
  }
  if(res == -1)
  {
    msg.key[0] = 'b';
    struct node* client = find_by_id(id1);
    if(client != NULL)
    {
      INT_block();
      mqd_t cli_queue = mq_open(client->key, O_WRONLY);
      mq_send(cli_queue, (char*)&msg, sizeof(struct msg_to_client), CONNECT_CLI);
      mq_close(cli_queue);
      INT_unblock();
    }
    return;
  }
    INT_block();
    mqd_t cli_queue1 = mq_open(key1, O_WRONLY);
    mqd_t cli_queue2 = mq_open(key2, O_WRONLY);
    strcpy(msg.key,key2);
    mq_send(cli_queue1, (char*)&msg, sizeof(struct msg_to_client), CONNECT_CLI);
    strcpy(msg.key,key1);
    mq_send(cli_queue2, (char*)&msg, sizeof(struct msg_to_client), CONNECT_CLI);
    mq_close(cli_queue1);
    mq_close(cli_queue2);
    INT_unblock();
}

void sig_int_handler(int sig)
{
  int size = head->connected;
  struct node* actual = head;
  struct msg_to_client msg;
  msg.mtype = STOP_CLI;
  for(int i = 0; i<size;i++)
  {
    actual = actual->next;
    mqd_t cli_queue = mq_open(actual->key, O_WRONLY);
    mq_send(cli_queue, (char*)&msg, sizeof(struct msg_to_client), STOP_CLI);
    mq_close(cli_queue);
  }
  while(head->connected > 0)
  {
    struct msg_to_server receive;
    if(mq_receive(queue, (void*)&receive, sizeof(struct msg_to_server),NULL)>0)
    {
      delete_from_list(receive.my_id);
    }
  }
  mq_close(queue);
  printf("%d\n", mq_unlink(server_key));
  exit(0);
}

int main()
{
  strcpy(server_key, "/server");
  struct sigaction act1;
  struct mq_attr attr1;
  attr1.mq_msgsize = sizeof(struct msg_to_server);
  attr1.mq_maxmsg = 10;
  sigemptyset(&act1.sa_mask);
  act1.sa_flags = 0;
  act1.sa_handler = sig_int_handler;
  sigaction(SIGINT, &act1, NULL);
  queue = mq_open(server_key, O_CREAT|O_WRONLY, 0777, &attr1);
  mq_close(queue);
  queue = mq_open(server_key, O_RDONLY);
  head = create_list();
  struct msg_to_server receive;
  while(1)
  {
    if(mq_receive(queue, (char *)&receive, sizeof(struct msg_to_server), NULL)>0)
    {
      if(receive.mtype == STOP_TO_SRV)
      {
        delete_from_list(receive.my_id);
      }
      if(receive.mtype == DISCONNECT_CHAT)
      {
        disconnect_clients(receive.my_id);
      }
      if(receive.mtype == LIST_CLIENTS)
      {
        send_registered_clients_id(receive);
      }
      if(receive.mtype == CONNECT_TO)
      {
        create_connection(receive);
      }
      if(receive.mtype == INIT)
      {
        send_init_msg(receive);
      }
    }
  }
}
