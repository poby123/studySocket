#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 100
void error_handling(const char *message);
void read_child_proccess(int signal);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  struct sigaction action;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  int state = sigaction(SIGCHLD, &action, 0);

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(atoi(argv[1]));

  int server_socket = socket(PF_INET, SOCK_STREAM, 0);

  if (bind(server_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    error_handling("bind() error");
  }

  if (listen(server_socket, 5) < 0) {
    error_handling("listen() error");
  }

  int fd[2];
  pipe(fd);

  pid_t pid = fork();

  // This is write file process.
  if (pid == 0) {
    close(fd[1]);

    FILE *fp = fopen("echo.txt", "wt");
    fclose(fp);
    char buffer[BUF_SIZE] = {0};

    while (1) {
      FILE *fp = fopen("echo.txt", "at");
      int len = read(fd[0], buffer, BUF_SIZE);
      fwrite(buffer, 1, len, fp);
      fclose(fp);
    }

    close(fd[0]);
    return 0;
  }

  // This is normal server process.
  close(fd[0]);
  while (1) {
    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    int client_socket =
        accept(server_socket, (struct sockaddr *)&client_address,
               &client_address_size);

    if (client_socket == -1) {
      continue;
    }

    puts("New client connected...");

    pid_t pid = fork();

    // This is client handling process.
    if (pid == 0) {
      close(server_socket);

      int str_len = 0;
      char buffer[BUF_SIZE] = {0};
      while ((str_len = read(client_socket, buffer, BUF_SIZE)) != 0) {
        write(client_socket, buffer, str_len);
        write(fd[1], buffer, str_len);
      }

      close(client_socket);
      puts("Client disconnected...");
      return 0;
    }

    close(client_socket);
  }

  close(fd[1]);
  close(server_socket);
  return 0;
}

void read_child_proccess(int signal) {
  pid_t pid;
  int status;

  pid = waitpid(-1, &status, WNOHANG);
  printf("removed PID: %d\n", pid);
}

void error_handling(const char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}