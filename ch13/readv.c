#include <stdio.h>
#include <sys/uio.h>
#define BUF_SIZE 100

int main(int argc, char* argv[]) {
  const int vec_size = 2;
  struct iovec vec[vec_size];

  char buffer1[BUF_SIZE] = {0};
  vec[0].iov_base = buffer1;
  vec[0].iov_len = 5;

  char buffer2[BUF_SIZE] = {0};
  vec[1].iov_base = buffer2;
  vec[1].iov_len = BUF_SIZE;

  // standard input
  int str_len = readv(0, vec, vec_size);

  printf("Read bytes: %d\n", str_len);
  printf("First message: %s\n", buffer1);
  printf("Second message: %s\n", buffer2);
}