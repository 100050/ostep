#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include<fcntl.h>

int main() {
    
    int x = 100;
    int file = open("p2_file", O_WRONLY);
    int rc = fork();

    if (rc == 0) {
        printf("child: %d\n", file);
        write(file, "child", 5);
    }
    else {
        x = 300;
        write(file, "main", 4);
    }
}