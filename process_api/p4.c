#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    
    int rc = fork();

    if (rc == 0) {
        // exec + l: 리스트로 인자를 받음, 가변인자임
        // exec + v: 배열 형태로 인자를 받음
        // exec + p: 파일 이름으로 받음
        // exec + e: 환경 정보를 받음
        // execl("/bin/ls", NULL, NULL);
        // execle("/bin/ls", NULL, NULL);
        // execlp("/bin/ls", NULL, NULL);
        // execv("/bin/ls", NULL);
        // execve("/bin/ls", NULL, NULL);
        // execvp("/bin/ls", NULL);
    }
    else {
    }

    return 0;
}   