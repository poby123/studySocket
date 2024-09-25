/**
 * 하나의 파이프로 양방향 송수신이 가능할까?
 */

#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[]) {
  int fd[2];
  pipe(fd);

  pid_t pid = fork();
  if (pid == 0) {
    char str[] = "Who are you?";
    char buf[BUF_SIZE] = {0};

    write(fd[1], str, sizeof(str));

    // 아래 줄만 주석처리해도 올바르게 동작하지 않는다.
    // 하나의 파이프로 양방향 송수신은 사실상 불가능하다.
    sleep(3);
    read(fd[0], buf, BUF_SIZE);
    printf("Child process read string: %s\n", buf);
  }

  char str[] = "Thank you for your message.";
  char buf[BUF_SIZE] = {0};
  read(fd[0], buf, BUF_SIZE);
  printf("Parent process read string: %s\n", buf);
  write(fd[1], str, sizeof(str));

  return 0;
}