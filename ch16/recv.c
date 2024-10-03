#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

void error_handling(const char*);

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage : %s <ip> <port> \n", argv[0]);
    exit(1);
  }

  int sock = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(argv[1]);
  server_address.sin_port = htons(atoi(argv[2]));

  connect(sock, (struct sockaddr*)&server_address, sizeof(server_address));

  FILE* read_fp = fdopen(sock, "r");
  FILE* write_fp = fdopen(sock, "w");

  while (1) {
    char buffer[BUF_SIZE] = {0};
    if (fgets(buffer, sizeof(buffer), read_fp) == NULL) {
      break;
    }

    fputs(buffer, stdout);
    fflush(stdout);
  }

  fputs("FROM CLIENT: Thank you!\n", write_fp);
  fflush(write_fp);

  fclose(write_fp);
  fclose(read_fp);

  return 0;
}

void error_handling(const char* msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}