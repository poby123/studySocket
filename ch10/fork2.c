#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int status;
  pid_t pid = fork();

  if (pid == 0) {
    return 3;
  }

  printf("Child PID: %d\n", pid);
  pid = fork();

  if (pid == 0) {
    sleep(5);
    exit(7);
  }

  printf("Child PID: %d\n", pid);
  wait(&status);
  if (WIFEXITED(status)) {
    printf("Child send one: %d\n", WEXITSTATUS(status));
  }

  /*
      wait는 임의의 자식 프로세스가 종료될 때까지 블로킹 상태에 놓이게 된다.
      이 때문에 함수 호출에 주의해야한다.
  */
  wait(&status);
  if (WIFEXITED(status)) {
    printf("Child send two: %d\n", WEXITSTATUS(status));
  }
  return 0;
}