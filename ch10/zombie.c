/**
 * ps au를 통해 좀비가 나오는 것을 볼 수 있다.
 */

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
  sleep(30);
  return 0;
}