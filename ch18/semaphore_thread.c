#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

void* access(void*);
void* accumulate(void*);

static sem_t sem_one;
static sem_t sem_two;
static int num;

int main() {
  sem_init(&sem_one, 0, 0);
  sem_init(&sem_two, 0, 1);

  pthread_t pt1, pt2;
  pthread_create(&pt1, NULL, access, NULL);
  pthread_create(&pt2, NULL, accumulate, NULL);

  pthread_join(pt1, NULL);
  pthread_join(pt2, NULL);

  sem_destroy(&sem_one);
  sem_destroy(&sem_two);

  return 0;
}

void* access(void* arg) {
  for (int i = 0; i < 5; i++) {
    fputs("Input num: ", stdout);

    sem_wait(&sem_two);
    scanf("%d", &num);
    sem_post(&sem_one);
  }

  return NULL;
}

void* accumulate(void* arg) {
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sem_wait(&sem_one);
    sum += num;
    sem_post(&sem_two);
  }

  printf("Result: %d\n", sum);
  return NULL;
}