#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

void error_handling(const char *);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
    exit(1);
  }

  int recv_socket = socket(PF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(atoi(argv[2]));

  if (bind(recv_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    error_handling("bind error()");
  }

  struct ip_mreq join_address;
  join_address.imr_multiaddr.s_addr = inet_addr(argv[1]);
  join_address.imr_interface.s_addr = htonl(INADDR_ANY);

  setsockopt(recv_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&join_address,
             sizeof(join_address));

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