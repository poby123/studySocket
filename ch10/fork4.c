#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void read_child_signal(int signal) {
  int status;
  pid_t pid = waitpid(-1, &status, WNOHANG);
  if (WIFEXITED(status)) {
    printf("Removed PID: %d\n", pid);
    printf("Child send: %d\n", WEXITSTATUS(status));
  }
}

int main(int argc, char* argv[]) {
  struct sigaction action;

  action.sa_handler = read_child_signal;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  sigaction(SIGCHLD, &action, 0);

  pid_t pid = fork();
  if (pid == 0) {
    puts("Hi! I'm child process");
    sleep(10);
    return 12;
  }

  printf("Child PID: %d\n", pid);
  pid = fork();

  if (pid == 0) {
    puts("Hi! I'm child process");
    sleep(10);
    exit(24);
  }

  printf("Child PID: %d\n", pid);
  for (int i = 0; i < 5; i++) {
    puts("wait...");
    sleep(5);
  }

  return 0;
}