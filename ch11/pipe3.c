/**
 * 파이프로 양방향 읽고/쓰기를 하려면 파이프를 2개 만들면 된다.
 */

#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[]) {
  int fd1[2], fd2[2];
  pipe(fd1), pipe(fd2);

  pid_t pid = fork();
  if (pid == 0) {
    char str[] = "Who are you?";
    char buf[BUF_SIZE] = {0};

    write(fd1[1], str, sizeof(str));
    read(fd2[0], buf, BUF_SIZE);
    printf("Child process read string: %s\n", buf);
  }

  char str[] = "Thank you for your message.";
  char buf[BUF_SIZE] = {0};
  read(fd1[0], buf, BUF_SIZE);
  printf("Parent process read string: %s\n", buf);
  write(fd2[1], str, sizeof(str));

  return 0;
}