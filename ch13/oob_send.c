#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30
void error_handling(const char* message);

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage : %s <IP> <PORT>\n", argv[0]);
    exit(1);
  }

  int sock = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in recv_address;
  memset(&recv_address, 0, sizeof(recv_address));
  recv_address.sin_family = AF_INET;
  recv_address.sin_addr.s_addr = inet_addr(argv[1]);
  recv_address.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr*)&recv_address, sizeof(recv_address)) <
      0) {
    error_handling("connect() error");
  }

  write(sock, "123", strlen("123"));
  send(sock, "4", strlen("4"), MSG_OOB);
  write(sock, "567", strlen("567"));
  send(sock, "8", strlen("8"), MSG_OOB);

  close(sock);

  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}