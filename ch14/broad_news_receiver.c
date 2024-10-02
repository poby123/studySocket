#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

void error_handling(const char *);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <PORT>\n", argv[0]);
    exit(1);
  }

  int recv_socket = socket(PF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(atoi(argv[1]));

  if (bind(recv_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    error_handling("bind error()");
  }

  while (1) {
    char buf[BUF_SIZE] = {0};
    int str_len = recvfrom(recv_socket, buf, BUF_SIZE - 1, 0, NULL, 0);
    if (str_len < 0) {
      break;
    }
    fputs(buf, stdout);
  }

  close(recv_socket);

  return 0;
}

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}