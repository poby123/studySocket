#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

void error_handling(const char*);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port> \n", argv[0]);
    exit(1);
  }

  int server_socket = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(atoi(argv[1]));

  if (bind(server_socket, (struct sockaddr*)&server_address,
           sizeof(server_address)) == -1) {
    error_handling("bind() error");
  }

  if (listen(server_socket, 5) == -1) {
    error_handling("listen() error");
  }

  struct sockaddr_in client_address;
  socklen_t client_address_size = sizeof(client_address);

  int client_socket = accept(server_socket, (struct sockaddr*)&client_address,
                             &client_address_size);

  FILE* read_fp = fdopen(client_socket, "r");
  FILE* write_fp = fdopen(client_socket, "w");

  fputs("FROM SERVER: HI CLIENT?\n", write_fp);
  fputs("FROM SERVER: I LOVE ALL OF THE WORLD\n", write_fp);
  fputs("FROM SERVER: YOU ARE AWESOME\n", write_fp);
  fflush(write_fp);
  fclose(write_fp);  // 쓰기만 종료되는 Half-close가 진행될까? 그렇다면 아래에서
                     // 수신이 가능할 것이다.

  char buffer[BUF_SIZE] = {0};
  fgets(buffer, sizeof(buffer), read_fp);
  fputs(buffer, stdout);
  fclose(read_fp);

  return 0;
}

void error_handling(const char* msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}