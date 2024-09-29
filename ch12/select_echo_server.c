#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 100
void error_handling(const char*);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  int server_socket = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(atoi(argv[1]));

  if (bind(server_socket, (struct sockaddr*)&server_address,
           sizeof(server_address)) < 0) {
    error_handling("bind() error");
  }

  if (listen(server_socket, 5) < 0) {
    error_handling("listen() error");
  }

  fd_set reads;
  FD_ZERO(&reads);
  FD_SET(server_socket, &reads);
  int fd_max = server_socket;

  while (1) {
    fd_set temps = reads;

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;

    int result = select(fd_max + 1, &temps, 0, 0, &timeout);
    if (result < 0) {
      break;
    }

    if (result == 0) {
      continue;
    }

    for (int i = 0; i < fd_max + 1; i++) {
      if (FD_ISSET(i, &temps) <= 0) {
        continue;
      }

      // SERVER
      if (i == server_socket) {
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);

        int client_socket =
            accept(server_socket, (struct sockaddr*)&client_address,
                   &client_address_size);

        FD_SET(client_socket, &reads);
        fd_max = max(fd_max, client_socket);

        printf("Connected client: %d\n", client_socket);
        continue;
      }

      // CLIENT
      char buffer[BUF_SIZE] = {0};
      int str_len = read(i, buffer, BUF_SIZE);
      if (str_len <= 0) {
        FD_CLR(i, &reads);
        close(i);
        printf("Closed client: %d\n", i);
        continue;
      }

      write(i, buffer, str_len);
    }
  }

  close(server_socket);
  return 0;
}

void error_handling(const char* message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}