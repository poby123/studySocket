#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define TTL 64
#define BUF_SIZE 30

void error_handling(const char*);

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage : %s <IP> <PORT>\n", argv[0]);
    exit(1);
  }

  int send_socket = socket(PF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in multicast_address;
  memset(&multicast_address, 0, sizeof(multicast_address));
  multicast_address.sin_family = AF_INET;
  multicast_address.sin_addr.s_addr = inet_addr(argv[1]);
  multicast_address.sin_port = htons(atoi(argv[2]));

  int time_live = TTL;
  setsockopt(send_socket, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live,
             sizeof(time_live));

  FILE* fp = NULL;
  fp = fopen("news.txt", "r");
  if (fp == NULL) {
    error_handling("fopen() error");
  }

  while (!feof(fp)) {
    char buf[BUF_SIZE] = {0};
    fgets(buf, BUF_SIZE, fp);
    sendto(send_socket, buf, strlen(buf), 0,
           (struct sockaddr*)&multicast_address, sizeof(multicast_address));
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