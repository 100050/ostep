#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }
    else if (argc == 2) {
        char* buffer = NULL;
        size_t len = 0;
        while(getline(&buffer, &len, stdin) != -1) {
            char* ptr = strstr(buffer, argv[1]);
            if (ptr == NULL) continue;

            printf("%s", buffer);
            free(buffer); 
        }

        return 0;
    }

    for (int i = 2; i < argc; i++) {
        FILE* fp = fopen(argv[i], "r");

        if (fp == NULL) {
            printf("wgrep: cannot open file\n");
            
            return 1;
        }
        
        char* buffer = NULL;
        size_t len = 0;
        while(getline(&buffer, &len, fp) != -1) {
            char* ptr = strstr(buffer, argv[1]);
            if (ptr == NULL) continue;

            printf("%s", buffer);
            free(buffer); 
        }
        
        fclose(fp);

    }


    return 0;
}