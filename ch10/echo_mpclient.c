#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 30
void error_handling(const char*);
void read_routine(int sock, char*);
void write_routine(int sock, char*);

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  int sock = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(argv[1]);
  server_address.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr*)&server_address,
              sizeof(server_address)) == -1) {
    error_handling("connect() error");
  }

  pid_t pid = fork();

  if (pid == 0) {
    char buf[BUF_SIZE] = {0};
    write_routine(sock, buf);
    return 0;
  }

  char buf[BUF_SIZE] = {0};
  read_routine(sock, buf);

  close(sock);

  return 0;
}

void read_routine(int sock, char* buf) {
  while (1) {
    int str_len = read(sock, buf, BUF_SIZE);
    if (str_len == 0) return;

    buf[str_len] = 0;
    printf("Message from server: %s", buf);
  }
}

void write_routine(int sock, char* buf) {
  while (1) {
    fgets(buf, BUF_SIZE, stdin);
    if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
      shutdown(sock, SHUT_WR);
      return;
    }

    write(sock, buf, strlen(buf));
  }
}

void error_handling(const char* message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}