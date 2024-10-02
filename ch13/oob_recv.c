#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

void error_handling(const char *);
void urg_handler(int);

int recv_sock, accept_sock;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <PORT>\n", argv[0]);
    exit(1);
  }

  struct sigaction action;
  action.sa_handler = urg_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  accept_sock = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in recv_address;
  recv_address.sin_family = AF_INET;
  recv_address.sin_addr.s_addr = htonl(INADDR_ANY);
  recv_address.sin_port = htons(atoi(argv[1]));

  if (bind(accept_sock, (struct sockaddr *)&recv_address,
           sizeof(recv_address)) < 0) {
    error_handling("bind() error");
  }

  if (listen(accept_sock, 5) < 0) {
    error_handling("listen() error");
  }

  struct sockaddr_in serv_address;
  socklen_t serv_address_size = sizeof(serv_address);
  recv_sock =
      accept(accept_sock, (struct sockaddr *)&serv_address, &serv_address_size);

  fcntl(recv_sock, F_SETOWN, getpid());
  int state = sigaction(SIGURG, &action, 0);

  int str_len = 0;
  char buffer[BUF_SIZE] = {0};
  while ((str_len = recv(recv_sock, buffer, sizeof(buffer), 0)) != 0) {
    if (str_len < 0) {
      continue;
    }

    buffer[str_len] = 0;
    puts(buffer);
  }

  close(recv_sock);
  close(accept_sock);

  return 0;
}

void urg_handler(int signal) {
  char buffer[BUF_SIZE] = {0};
  int str_len = recv(recv_sock, buffer, sizeof(buffer) - 1, MSG_OOB);
  buffer[str_len] = 0;
  printf("Urgent message: %s\n", buffer);
}

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}