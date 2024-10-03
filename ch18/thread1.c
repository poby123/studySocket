#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* thread_main(void* arg);

int main(int argc, char* argv[]) {
  pthread_t t_id;
  int thread_param = 5;

  if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_param)) {
    perror("pthread_create() error");
    return -1;
  }

  void* thread_return;
  if (pthread_join(t_id, &thread_return)) {
    perror("pthread_join() error");
    return -1;
  }

  printf("Message from thread: %s", (char*)thread_return);
  puts("end of main");

  free(thread_return);

  return 0;
}

void* thread_main(void* arg) {
  int count = *((int*)arg);

  char* message = (char*)malloc(sizeof(char) * 50);
  strcpy(message, "Hello, I'am thread\n");

  for (int i = 0; i < count; i++) {
    sleep(1);
    puts("running thread");
  }

  return message;
}