#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30

void error_handling(const char*);

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage : %s <IP> <PORT>\n", argv[0]);
    exit(1);
  }

  int send_socket = socket(PF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in broadcast_address;
  memset(&broadcast_address, 0, sizeof(broadcast_address));
  broadcast_address.sin_family = AF_INET;
  broadcast_address.sin_addr.s_addr = inet_addr(argv[1]);
  broadcast_address.sin_port = htons(atoi(argv[2]));

  int so_broad = 1;
  setsockopt(send_socket, SOL_SOCKET, SO_BROADCAST, (void*)&so_broad,
             sizeof(so_broad));

  FILE* fp = NULL;
  fp = fopen("news.txt", "r");
  if (fp == NULL) {
    error_handling("fopen() error");
  }

  while (!feof(fp)) {
    char buf[BUF_SIZE] = {0};
    fgets(buf, BUF_SIZE, fp);
    sendto(send_socket, buf, strlen(buf), 0,
           (struct sockaddr*)&broadcast_address, sizeof(broadcast_address));
    sleep(1);
  }

  fclose(fp);
  close(send_socket);

  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}