#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int status;
  pid_t pid = fork();

  if (pid == 0) {
    sleep(15);
    return 24;
  }

  /**
   * waitpid(pid_t pid, int* statloc, int options);
   * 첫 번째는 종료를 확인하고자 하는 자식 프로세스의 PID이다.
   * -1이면 아무거나 기다린다.
   *
   * 두번째는 status 전달을 위한 인자이다.
   *
   * 세번째에는 WNOHANG을 전달하면, 종료된 자식 프로세스가 존재하지 않으면
   * 블로킹 상태에 있지 않고, 0을 반환한다. 블로킹 상태에 있지 않지만 주기적인
   * 체크가 필요하다. 그런데, 부모 프로세스도 바쁜데 언제 이렇게 체크하고
   * 앉아있을까? fork4에서 시그널링을 이용해서 해결한다.
   */
  while (!waitpid(pid, &status, WNOHANG)) {
    sleep(3);
    puts("sleep 3 sec.");
  }

  if (WIFEXITED(status)) {
    printf("Child send: %d\n", WEXITSTATUS(status));
  }

  return 0;
}