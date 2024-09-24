#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void timeout(int signal) {
  if (signal == SIGALRM) {
    puts("Timeout!");
  }
  alarm(2);
}

void key_control(int signal) {
  if (signal == SIGINT) {
    puts("CTRL+C pressed");
  }
}

int main(int argc, char* argv[]) {
  signal(SIGALRM, timeout);
  signal(SIGINT, key_control);
  alarm(2);

  for (int i = 0; i < 3; i++) {
    puts("wait...");

    /**
     * 시그널이 생기면 sleep은 깨진다. 그래서 이 프로그램은 10초안에 끝난다.
     */
    sleep(100);
  }

  return 0;
}