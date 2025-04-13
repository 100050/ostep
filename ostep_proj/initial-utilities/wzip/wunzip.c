#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }
    else if (argc == 2) {
        FILE* readFp = fopen(argv[1], "rb");

        if (readFp == NULL) {
            printf("wunzip: cannot open file\n");
            
            return 1;
        }

        char* buffer = NULL;
        char prev;
        int size;

        while(!feof(readFp)) {
            fread(&size, sizeof(int), 1, readFp);
            fread(&prev, sizeof(char), 1, readFp);

            for (int i = 0; i < size; i++) {
                printf("%c", prev);
            }
        }

        free(buffer); 

        fclose(readFp);
    }
    

    return 0;
}   