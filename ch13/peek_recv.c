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

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <PORT>\n", argv[0]);
    exit(1);
  }

  int accept_sock = socket(PF_INET, SOCK_STREAM, 0);
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
  int recv_sock =
      accept(accept_sock, (struct sockaddr *)&serv_address, &serv_address_size);

  char buffer[BUF_SIZE] = {0};
  int str_len = 0;
  while (1) {
    /**
     * MSG_PEEK을 통해 수신할 데이터가 존재하는 지 확인할 수 있고,
     * MSG_DONTWAIT를 통해 수신할 데이터 여부와 상관없이
     * 블로킹 되지 않고 확인할 수 있다.
     */
    str_len =
        recv(recv_sock, buffer, sizeof(buffer) - 1, MSG_PEEK | MSG_DONTWAIT);
    if (0 < str_len) {
      break;
    }
  }
  printf("Buffering: %d bytes: %s\n", str_len, buffer);

  str_len = recv(recv_sock, buffer, sizeof(buffer) - 1, 0);
  buffer[str_len] = 0;

  printf("Read again: %s\n", buffer);

  close(accept_sock);
  close(recv_sock);

  return 0;
}

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}