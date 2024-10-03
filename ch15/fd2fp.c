#include <fcntl.h>
#include <stdio.h>

int main() {
  int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0) {
    perror("file open error");
    return -1;
  }

  printf("fd: %d\n", fd);

  // 쓰기 모드의 파일 포인터를 반환합니다.
  FILE* fp = fdopen(fd, "w");
  fputs("Network C Programming\n", fp);

  int fd2 = fileno(fp);
  printf("fd2: %d\n", fd2);

  // 파일 자체가 완전히 종료되는 것이므로,
  // 파일 디스크립터로 또 다시 종료할 필요는 없다.
  fclose(fp);

  return 0;
}