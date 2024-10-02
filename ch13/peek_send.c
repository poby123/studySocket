#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void error_handling(const char*);

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage : %s <IP> <PORT>\n", argv[0]);
    exit(1);
  }

  int sock = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in send_address;
  memset(&send_address, 0, sizeof(send_address));
  send_address.sin_family = AF_INET;
  send_address.sin_addr.s_addr = inet_addr(argv[1]);
  send_address.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr*)&send_address, sizeof(send_address)) <
      0) {
    error_handling("connect() error");
  }

  write(sock, "123", strlen("123"));
  close(sock);

  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}