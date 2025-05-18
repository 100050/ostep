#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int s = 0;
    char *name = ".";
    if (argc == 3 && !strcmp(argv[1], "-l")) {
        s = 1;
        name = argv[2];
    }
    else if (argc == 2 && !strcmp(argv[1], "-l")) {
        s = 1;
    }
    else if (argc==2) {
        name = argv[1];
    }
    
    DIR *d = opendir(name);
    if (d == NULL) perror("opendir error\n");

    struct dirent* dn = NULL;
    while ((dn = readdir(d)) != NULL) {
        printf("%s\n", dn->d_name);

        if (s) { // 상세 정보 출력
            struct stat sb;
            char path[10000] = "";
            strcpy(path, name);
            if(stat(strcat(strcat(path, "/"), dn->d_name), &sb) == -1) {
                perror("stat error\n");
                continue;
            }
            
            printf("UID=%d, GID=%d, %d\n", sb.st_uid, sb.st_gid, sb.st_mode);

        }
    }

    closedir(d);


    return 0;
}