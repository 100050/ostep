#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }
    else if (argc == 2 || argv[argc-2][0] != '>') {
        for (int i = 1; i < argc; i++) {
            FILE* readFp = fopen(argv[i], "r");

            if (readFp == NULL) {
                printf("wzip: cannot open file\n");
                
                return 1;
            }

            char* buffer = NULL;
            char prev;
            int size;
            size_t len = 0;
            while(getline(&buffer, &len, readFp) != -1) {
                prev = buffer[0];
                size = 1;
                for (int j = 1; j < len; j++) {
                    if (prev != buffer[j]) {
                        fwrite(&size, sizeof(int), 1, stdout);
                        fwrite(&prev, sizeof(char), 1, stdout);
                        prev = buffer[j];
                        size = 1;
                    }
                    else {
                        size++;
                    }
                }
                
            }
            free(buffer); 

            fclose(readFp);
        }


        return 0;
    }
    else if (argv[argc-2][0] == '>') {
        FILE* writeFp = fopen(argv[argc-1], "wb");
        for (int i = 1; i < argc-2; i++) {
            FILE* readFp = fopen(argv[i], "r");

            if (readFp == NULL) {
                printf("wzip: cannot open file\n");
                
                return 1;
            }

            char* buffer = NULL;
            char prev;
            int size;
            size_t len = 0;
            while(getline(&buffer, &len, readFp) != -1) {
                prev = buffer[0];
                size = 1;
                for (int j = 1; j < len; j++) {
                    if (prev != buffer[j]) {
                        fwrite(&size, sizeof(int), 1, writeFp);
                        fwrite(&prev, sizeof(char), 1, writeFp);
                        prev = buffer[j];
                        size = 1;
                    }
                    else {
                        size++;
                    }
                }
                
            }
            free(buffer); 

            fclose(readFp);
        }

        fclose(writeFp);
    }

    return 0;
}   