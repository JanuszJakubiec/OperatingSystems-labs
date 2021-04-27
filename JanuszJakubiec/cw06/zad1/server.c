#include "header.h"

struct node
{
  int id;
  int key;
  int connected;
  struct node* next;
  struct node* prev;
};

struct node* head;
int queue;



void send_disconnect_message(int key)
{
  struct msg_to_client msg;
  msg.mtype = CONNECT_CLI;
  msg.new_chat_key = 0;
  int queue = msgget(key, IPC_EXCL);
  msgsnd(queue, (void*)&msg, sizeof(struct msg_to_client) - sizeof(long), 0);
}

struct node* create_list()
{
  struct node* head;
  head = calloc(1, sizeof(struct node));
  head->id = 0;
  head->key = -1;
  head->connected = 0;
  head->next = NULL;
  head->prev = NULL;
  return head;
}

int add_to_list(int key)
{
  struct node* actual = head;
  for(int i = 0; i<head->connected;i++)
  {
    actual = actual->next;
  }
  struct node* new = calloc(1, sizeof(struct node));
  new->id = actual->id + 1;
  new->key = key;
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
    return -1;
  send_disconnect_message(c1->key);
  if(c1->connected == 0)
  {
    return -2;
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
  //int key = disconnect_clients(id);
  //if(key > 0)
  //{
  //  send_disconnect_message(key);
  //}
  disconnect_clients(id);
  actual->prev->next = actual->next;
  if(actual->next != NULL)
    actual->next->prev = actual->prev;
  free(actual);
  head->connected -= 1;
  return 0;
}

int get_key(int id)
{
  struct node* actual = find_by_id(id);
  if(actual == NULL)
    return -1;
  return actual->key;
}

int connect_clients(int id1, int id2, int* key1, int* key2)
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
  *key1 = c1->key;
  *key2 = c2->key;
  return 0;
}

void send_init_msg(struct msg_to_server received)
{
  int id = add_to_list(received.my_key);
  struct msg_to_client msg;
  msg.mtype = INIT_CLI;
  msg.your_id = id;
  int client_queue = msgget(received.my_key, IPC_EXCL);
  msgsnd(client_queue, (void*)&msg, sizeof(struct msg_to_client) - sizeof(long), 0);
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
    int client_queue = msgget(client->key, IPC_EXCL);
    msgsnd(client_queue, (void*)&msg, sizeof(struct msg_to_client) - sizeof(long), 0);
  }
}

void create_connection(struct msg_to_server received)
{
  int key1, key2;
  int id1, id2;
  id1 = received.my_id;
  id2 = received.connect_to;
  int res = connect_clients(id1, id2, &key1, &key2);
  struct msg_to_client msg;
  msg.mtype = CONNECT_CLI;
  if(res == -2)
  {
    msg.new_chat_key = -2;
    struct node* client = find_by_id(id1);
    if(client != NULL)
    {
      int client_queue = msgget(client->key, IPC_EXCL);
      msgsnd(client_queue, (void*)&msg, sizeof(struct msg_to_client) - sizeof(long), 0);
    }
    return;
  }
  if(res == -1)
  {
    msg.new_chat_key = -1;
    struct node* client = find_by_id(id1);
    if(client != NULL)
    {
      int client_queue = msgget(client->key, IPC_EXCL);
      msgsnd(client_queue, (void*)&msg, sizeof(struct msg_to_client) - sizeof(long), 0);
    }
    return;
  }
  int client1_queue = msgget(key1, IPC_EXCL);
  int client2_queue = msgget(key2, IPC_EXCL);
  msg.new_chat_key = key2;
  msgsnd(client1_queue, (void*)&msg, sizeof(struct msg_to_client) - sizeof(long), 0);
  msg.new_chat_key = key1;
  msgsnd(client2_queue, (void*)&msg, sizeof(struct msg_to_client) - sizeof(long), 0);
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
    int client_queue = msgget(actual->key, IPC_EXCL);
    msgsnd(client_queue, (void*)&msg, sizeof(struct msg_to_client) - sizeof(long), 0);
  }
  while(head->connected > 0)
  {
    struct msg_to_server receive;
    if(msgrcv(queue, (void*)&receive, sizeof(struct msg_to_server) - sizeof(long), STOP_TO_SRV, 0)>0)
    {
      delete_from_list(receive.my_id);
    }
  }
  msgctl(queue, IPC_RMID, NULL);
  exit(0);
}

int main()
{
  struct sigaction act1;
  sigemptyset(&act1.sa_mask);
  act1.sa_flags = 0;
  act1.sa_handler = sig_int_handler;
  sigaction(SIGINT, &act1, NULL);
  int my_key = ftok(HOME, 's');
  queue = msgget(my_key, 0777| IPC_CREAT | IPC_EXCL);
  head = create_list();
  struct msg_to_server receive;
  while(1)
  {
    if(msgrcv(queue, (void*)&receive, sizeof(struct msg_to_server) - sizeof(long), -7, 0)>0)
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
