#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]) {
  int sock;
  char message[BUF_SIZE];
  int str_len, recv_len, recv_cnt;
  struct sockaddr_in serv_addr;

  if (argc != 3) {
    printf("Usage : %s <IP> <port>\n", argv[0]);
    exit(1);
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    error_handling("socket() error");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    error_handling("connect() error");
  } else {
    puts("Connected...");
  }

  while (1) {
    fputs("Input message(Q to quit) : ", stdout);
    fgets(message, BUF_SIZE, stdin);

    if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
      break;
    }

    // 문자열이 길어져서, 서버에서 데이터를 쪼개서 보내는 경우 문제가 생긴다.
    // write(sock, message, strlen(message));
    // str_len = read(sock, message, BUF_SIZE - 1);
    // message[str_len] = 0;

    // 다음과 같은 코드가 바람직하다.
    str_len = write(sock, message, strlen(message));

    recv_len = 0;
    while (recv_len < str_len) {
      recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
      printf("recv_cnt : %d\n", recv_cnt);

      if (recv_cnt == -1) {
        error_handling("read() error");
      }
      recv_len += recv_cnt;
    }

    message[recv_len] = 0;
    printf("Message from server : %s \n", message);
  }

  close(sock);
  return 0;
}

void error_handling(char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}