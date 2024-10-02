#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char* argv[]) {
  const int vec_size = 2;
  struct iovec vec[vec_size];

  char buf1[] = "ABCDEFG";
  vec[0].iov_base = buf1;
  vec[0].iov_len = 3;

  char buf2[] = "1234567";
  vec[1].iov_base = buf2;
  vec[1].iov_len = 4;

  // standard output
  int str_len = writev(1, vec, vec_size);
  puts("");
  printf("Write bytes: %d\n", str_len);

  return 0;
}