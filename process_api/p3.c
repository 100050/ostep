#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    
    int rc = fork();

    if (rc == 0) {
        printf("hello\n");
    }
    else {
        for (int i = 0; i < 10000000; i++);
        printf("goodbye\n");
    }
}   