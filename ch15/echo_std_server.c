#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 20
void error_handling(const char*);
void read_child_proccess(int);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port> \n", argv[0]);
    exit(1);
  }

  struct sigaction action;
  action.sa_handler = read_child_proccess;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  sigaction(SIGCHLD, &action, 0);

  int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_address;

  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (struct sockaddr*)&server_address,
           sizeof(server_address)) == -1) {
    error_handling("bind() error");
  }

  if (listen(serv_sock, 5) == -1) {
    error_handling("listen() error");
  }

  while (1) {
    struct sockaddr_in client_address;
    socklen_t adr_sz = sizeof(client_address);

    int clnt_sock =
        accept(serv_sock, (struct sockaddr*)&client_address, &adr_sz);
    if (clnt_sock == -1) {
      continue;
    }
    puts("new client connected...");

    pid_t pid = fork();

    if (pid == -1) {
      close(clnt_sock);
      continue;
    }

    int strlen;
    char buf[BUF_SIZE];

    /* child process */
    if (pid == 0) {
      close(serv_sock);

      FILE* read_fp = fdopen(clnt_sock, "r");
      FILE* write_fp = fdopen(clnt_sock, "w");

      while (!feof(read_fp)) {
        fgets(buf, BUF_SIZE, read_fp);
        fputs(buf, write_fp);
        fflush(write_fp);
      }

      fclose(read_fp);
      fclose(write_fp);
      puts("client disconnected...");
      return 0;
    }

    /* parent process */
    close(clnt_sock);
  }

  close(serv_sock);
  return 0;
}

void read_child_proccess(int sig) {
  pid_t pid;
  int status;
  pid = waitpid(-1, &status, WNOHANG);
  printf("removed PID: %d\n", pid);
}

void error_handling(const char* msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}