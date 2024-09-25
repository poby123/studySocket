/**
 * 기본적인 파이프 예시
 */

#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#define BUF_SIZE 20

int main(int argc, char* argv[]) {
  int fd[2];
  pipe(fd);

  pid_t pid = fork();

  if (pid == 0) {
    char message[] = "Who are you?";
    write(fd[1], "Who are you?", sizeof(message));
    return 0;
  }

  char buf[BUF_SIZE] = {0};
  read(fd[0], buf, BUF_SIZE);
  puts(buf);

  return 0;
}
