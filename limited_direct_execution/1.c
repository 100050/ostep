#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // clang -o ./1.out ./1.c
  // sudo perf stat ./1.out

  // measure system call
  // 파일 생성 | 읽기 전용, 소유자에게 모든 권한 허용
  // S_I R: read, W:write, X:excute, U: owner, G: group, O: other
  int fd = open("./1.txt", O_CREAT | O_RDONLY, S_IRWXU), nloops = 1000000;

  struct timeval start, end;
  gettimeofday(&start, NULL); // 시작 시간
  for (size_t i = 0; i < nloops; i++) {
    read(fd, NULL, 0);
  }
  gettimeofday(&end, NULL); // 종료 시간

  // gettimeofday 함수를 통해 마이크로초까지 시간을 잰다
  printf("system call: %f microseconds\n\n",
         (float)(end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 -
                 start.tv_usec) /
             nloops);
  close(fd);

  // measure context switch
  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(sched_getcpu(), &set); // 현재 실행 중인 CPU 번호 반환
  if (sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1) { // 이 스레드가 해당 CPU 위에서 돌도록 설정
    perror("sched_setaffinity");
    exit(EXIT_FAILURE);
  }

  int first_pipefd[2], second_pipefd[2]; // 파이프 생성, 하나는 부모용, 하나는 자식용
  // 자식과 부모가 번갈아가며 io 사용, 문맥교환 시간을 잰다
  if (pipe(first_pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  if (pipe(second_pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  pid_t cpid = fork();
  if (cpid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (cpid == 0) { // child
    char buf = '0';
    close(first_pipefd[1]);
    close(second_pipefd[0]);
    for (size_t i = 0; i < nloops; i++) {
      if (read(first_pipefd[0], &buf, 1) == -1) {
        perror("child read");
        _exit(EXIT_FAILURE);
      }
      if (write(second_pipefd[1], &buf, 1) == -1) {
        perror("child write");
        _exit(EXIT_FAILURE);
      }
    }
    close(first_pipefd[0]);
    close(second_pipefd[1]);
    _exit(EXIT_SUCCESS);
  } else { // parent
    gettimeofday(&start, NULL);
    char buf = '0';
    close(first_pipefd[0]);
    close(second_pipefd[1]);
    for (size_t i = 0; i < nloops; i++) {
      if (write(first_pipefd[1], &buf, 1) == -1) {
        perror("parent write");
        exit(EXIT_FAILURE);
      }
      if (read(second_pipefd[0], &buf, 1) == -1) {
        perror("parent read");
        exit(EXIT_FAILURE);
      }
    }
    gettimeofday(&end, NULL);
    printf("context switch: %f microseconds\n",
           (float)(end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 -
                   start.tv_usec) /
               nloops / 2);
    wait(NULL);
    close(first_pipefd[1]);
    close(second_pipefd[0]);
    exit(EXIT_SUCCESS);
  }
}
