/**
 * The size of news.txt is 309MB.
 * Cost time: 182.774277 sec
 */

#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define BUF_SIZE 3

int main(int argc, char* argv[]) {
  int fd1 = open("news.txt", O_RDONLY);
  int fd2 = open("cpy.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

  const clock_t start = clock();

  int len = 0;
  char buf[BUF_SIZE] = {0};
  while ((len = read(fd1, buf, sizeof(buf))) > 0) {
    write(fd2, buf, len);
  }
  const clock_t end = clock();
  printf("Cost time: %f sec\n", ((end - start) / (float)CLOCKS_PER_SEC));

  close(fd1);
  close(fd2);

  return 0;
}