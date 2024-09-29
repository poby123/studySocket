#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
  fd_set reads;
  FD_ZERO(&reads);
  FD_SET(0, &reads);  // 0 is standard input

  while (1) {
    // select가 끝나면 변화가 생긴 부분 외의 모든 비트가 0으로 초기화되므로,
    // 이렇게 복사하여 사용하는 것이 일반적이다.
    fd_set temps = reads;

    // select가 끝나면 timeout도 타임아웃까지 남은 시간으로 바뀌기 때문에,
    // 호출할 때마다 설정해줘야한다.
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int result = select(1, &temps, 0, 0, &timeout);
    if (result < 0) {
      puts("select() error");
      break;
    }

    if (result == 0) {
      puts("Timeout!");
      continue;
    }

    // 표준입력에 변화가 생겼다면
    if (FD_ISSET(0, &temps)) {
      char buffer[BUF_SIZE] = {0};
      int str_len = read(0, buffer, BUF_SIZE);
      printf("message from console: %s", buffer);
    }
  }

  return 0;
}