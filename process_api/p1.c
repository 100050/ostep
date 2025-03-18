#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    
    int x = 100;
    int rc = fork();

    if (rc == 0) {
        printf("child: %d\n", x);
        x = 200;
        printf("child2: %d\n", x);
    }
    else {
        x = 300;
        printf("main: %d\n", x);
    }
}