#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREAD 100

void* inc(void*);
void* dec(void*);
long long count = 0LL;
pthread_mutex_t mutex;

int main(int argc, char* argv[]) {
  pthread_mutex_init(&mutex, NULL);
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

  pthread_mutex_destroy(&mutex);
  return 0;
}

void* inc(void* arg) {
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < 1e5; i++) {
    count += i;
  }
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void* dec(void* arg) {
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < 1e5; i++) {
    count -= i;
  }
  pthread_mutex_unlock(&mutex);
  return NULL;
}
