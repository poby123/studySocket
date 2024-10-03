/**
 * 동시성 문제의 재현
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREAD 100

void* inc(void*);
void* dec(void*);
long long count = 0LL;

int main(int argc, char* argv[]) {
  pthread_t thread_id_list[NUM_THREAD];
  for (int i = 0; i < NUM_THREAD; i++) {
    if (i % 2) {
      pthread_create(&thread_id_list[i], NULL, inc, NULL);
      continue;
    }
    pthread_create(&thread_id_list[i], NULL, dec, NULL);
  }

  for (int i = 0; i < NUM_THREAD; i++) {
    pthread_join(thread_id_list[i], NULL);
  }

  printf("result: %lld\n", count);
  return 0;
}

void* inc(void* arg) {
  for (int i = 0; i < 1e5; i++) {
    count += i;
  }
  return NULL;
}

void* dec(void* arg) {
  for (int i = 0; i < 1e5; i++) {
    count -= i;
  }
  return NULL;
}
