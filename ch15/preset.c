#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 3

int main(int argc, char* argv[]) {
  char buffer[] =
      "하지만 표준입출력 함수의 버퍼는 오로지 성능향상을 목적으로 한다. 그런데 버퍼링을 하면 어떻게 성능이 좋아지는걸까? 패킷에는 데이터의 크기와 상관없이 일정 크기의 헤더가 들어가기 때문에, 전송횟수가 성능에 영향을 미치기 때문이다. 예를 들어 10 바이트의 데이터를 보낸다고 해보자. 만약, 출력버퍼의 크기가 1B라면 1B씩 10번을 보내야한다. 그런데 데이터를 보낼 때는 일정 크기를 가지는 헤더도 같이 보내야 하므로, 헤더가 40B라면 총 40B x 10회 = 400B의 헤더 + 10B = 410B를 보내야한다. 하지만, 출력버퍼의 크기가 10B라면 40B + 10B = 50B만 보내면 되므로 데이터 양에 꽤나 큰 차이가 난다. \
또 전송횟수가 많아지면, 패킷을 소켓의 출력버퍼로 이동시키는데 걸리는 시간도 늘어난다. 1B를 10회 이동하는데 걸리는 시간이, 10B를 1회 이동하는 시간보다 약 10배 정도 더 소요된다.\n";

  int fd1 = open("news.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd1 < 0) {
    perror("Failed to open file");
    return 1;
  }

  for (int i = 0; i < 300000; i++) {
    write(fd1, buffer, strlen(buffer));
  }

  close(fd1);
  return 0;
}