#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main() {
    
    int rc = fork();

    if (rc == 0) {
        close(STDOUT_FILENO);
        printf("child\n");
    }
    else {
        printf("main\n");
    }

    return 0;
}   