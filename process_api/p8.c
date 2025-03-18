#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
// 두 개의 자식 프로세스를 생성하고 pipe() 시스템 콜을 사용하여 한 자식의 표준
// 출력을 다른 자식의 입력으로 연결하는 프로그램을 작성하라.

// ???????????????
int main() {
    
    int rc = fork();
    
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "error");
    }
    
    if (rc == 0) {
        printf("child\n");
        close(fd[0]);
        dup2(fd[1], 1);
        char l[10];
        scanf("%s", l);
        printf("%s", l);
    }
    else {
        //int c = wait(NULL);
        //printf("main %d\n", c);
        dup2(fd[0], 0);
        close(fd[1]);
        char line[10];
        
        int rc2 = fork();
        
        if (rc2 == 0) {
            scanf("%s", line); 
            printf("%s\n", line);
            
            
        }
        else {

        }
    }

    return 0;
}   