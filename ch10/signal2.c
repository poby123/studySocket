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
  struct sigaction action;

  action.sa_handler = timeout;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  sigaction(SIGALRM, &action, 0);

  alarm(2);
  for (int i = 0; i < 3; i++) {
    puts("wait...");
    sleep(100);
  }

  return 0;
}