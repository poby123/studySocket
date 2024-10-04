#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 100
#define MAX_CLIENT 256

void* handle_client(void*);
void send_message(const char* message, int length);
void error_handling(const char*);

int client_count = 0;
int client_socket_list[MAX_CLIENT];
pthread_mutex_t mutex;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  pthread_mutex_init(&mutex, NULL);
  int server_socket = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(atoi(argv[1]));

  if (bind(server_socket, (struct sockaddr*)&server_address,
           sizeof(server_address)) < 0) {
    error_handling("bind() error");
  }

  if (listen(server_socket, 5) < 0) {
    error_handling("listen() error");
  }

  while (1) {
    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_address,
                               &client_address_size);

    pthread_mutex_lock(&mutex);
    client_socket_list[client_count++] = client_socket;
    pthread_mutex_unlock(&mutex);

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_client, (void*)&client_socket);
    pthread_detach(thread_id);

    printf("Connected client IP: %s\n", inet_ntoa(client_address.sin_addr));
  }

  close(server_socket);
  pthread_mutex_destroy(&mutex);

  return 0;
}

void* handle_client(void* args) {
  int client_socket = *(int*)args;
  char message[BUF_SIZE] = {0};
  int str_len = 0;

  while ((str_len = read(client_socket, message, BUF_SIZE)) != 0) {
    send_message(message, str_len);
  }

  pthread_mutex_lock(&mutex);
  for (int i = 0; i < client_count; i++) {
    if (client_socket != client_socket_list[i]) {
      continue;
    }

    while (i + 1 < client_count) {
      client_socket_list[i] = client_socket_list[i + 1];
      ++i;
    }
    break;
  }
  --client_count;
  pthread_mutex_unlock(&mutex);

  close(client_socket);
  return NULL;
}

void send_message(const char* message, int length) {
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < client_count; i++) {
    write(client_socket_list[i], message, length);
  }
  pthread_mutex_unlock(&mutex);
}

void error_handling(const char* message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}
