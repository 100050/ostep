#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        FILE* fp = fopen(argv[i], "r");

        if (fp == NULL) {
            printf("wcat: cannot open file\n");
            
            return 1;
        }
        
        // 4 바이트씩 파일 읽기
        while (feof(fp) == 0) { // 파일의 끝이 되면 반복문 탈출
            char buffer[5] = "";
            fread(buffer, sizeof(char), 4, fp);
            printf("%s", buffer);
            memset(buffer, 0, 5); 
        }
        
        fclose(fp);

        printf("\n");
    }


    return 0;
}