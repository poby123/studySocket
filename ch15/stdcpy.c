/**
 * The size of news.txt is 309MB.
 * Cost time: 5.294210 sec
 */

#include <stdio.h>
#include <time.h>

#define BUF_SIZE 3

int main(int argc, char* argv[]) {
  FILE* fp1 = fopen("news.txt", "r");
  FILE* fp2 = fopen("cpy2.txt", "w");

  const clock_t start = clock();

  char buf[BUF_SIZE] = {0};
  while (fgets(buf, BUF_SIZE, fp1) != NULL) {
    fputs(buf, fp2);
  }

  const clock_t end = clock();
  printf("Cost time: %f sec\n", ((end - start) / (float)CLOCKS_PER_SEC));

  fclose(fp1);
  fclose(fp2);

  return 0;
}