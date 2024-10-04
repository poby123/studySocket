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
#define NAME_SIZE 20

void* send_message(void*);
void* recv_message(void*);
void error_handling(const char*);

char name[NAME_SIZE] = "[DEFAULT]";

int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("Usage : %s <IP> <port> <name>\n", argv[0]);
    exit(1);
  }

  sprintf(name, "%s", argv[3]);

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    error_handling("socket() error");
  }

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(argv[1]);
  server_address.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) <
      0) {
    error_handling("connect() error");
  }

  pthread_t read_thread;
  pthread_create(&read_thread, NULL, recv_message, (void*)&sock);

  pthread_t send_thread;
  pthread_create(&send_thread, NULL, send_message, (void*)&sock);

  void* thread_return;
  pthread_join(send_thread, &thread_return);
  pthread_join(read_thread, &thread_return);

  close(sock);
  return 0;
}

void* send_message(void* args) {
  int sock = (*(int*)args);

  while (1) {
    char buffer[BUF_SIZE] = {0};
    fgets(buffer, BUF_SIZE, stdin);
    if (!strcmp(buffer, "q\n") || !strcmp(buffer, "Q\n")) {
      close(sock);
      exit(0);
    }

    char message[NAME_SIZE + BUF_SIZE] = {0};
    sprintf(message, "%s %s", name, buffer);
    write(sock, message, strlen(message));
  }

  return NULL;
}

void* recv_message(void* args) {
  int sock = (*(int*)args);

  while (1) {
    char buffer[NAME_SIZE + BUF_SIZE] = {0};
    int str_len = read(sock, buffer, NAME_SIZE + BUF_SIZE);
    if (str_len < 0) {
      return (void*)-1;
    }

    fputs(buffer, stdout);
  }

  return NULL;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}