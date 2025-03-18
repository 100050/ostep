#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main() {
    
    int rc = fork();

    if (rc == 0) {
        printf("child\n");
        //int c = wait(NULL);
        //printf("%d\n", c);
        return 0;
    }
    else {
        int child = wait(NULL);
        printf("child return: %d\n", child);
    }

    return 0;
}   