#include <stdio.h>     // fprintf, perror
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>    // getopt
#include <getopt.h>
#include <stdlib.h>    // exit
#include <string.h>    // strncmp, strlen
#include <dirent.h>    // opendir, readdir, closedir
#include <errno.h>     // EACCES
#include <limits.h>    // INT_MAX
#include <regex.h>     // regcomp, regexec

#define STRINGSIZE 1024
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

void find_dir(char * pathname, int currentDepth, int maxDepth, regex_t * preg) { // dfs
    DIR *dp;
    struct dirent *d;
    errno = 0;
    if (currentDepth++ > maxDepth) // dfs 종료 조건
        return;
    if ((dp = opendir(pathname)) == NULL) {
        if (errno == EACCES) {
            fprintf(stderr, "myfind: '%s': Permission denied\n", pathname);
            return;
        } else {
            handle_error("opendir");
        }
    }
    while ((d = readdir(dp)) != NULL) { // 현재 노드의 자식 노드 탐색
        char filePath[STRINGSIZE] = "";
        strncpy(filePath, pathname, strlen(pathname));
        if (filePath[strlen(filePath) - 1] != '/')
            strncat(filePath, "/", 1);
        strncat(filePath, d->d_name, strlen(d->d_name));
        if (strncmp(d->d_name, ".", 1) != 0 && strncmp(d->d_name, "..", 2) != 0) {
            if (preg == NULL || regexec(preg, d->d_name, 0, NULL, 0) != REG_NOMATCH)
                printf("%s\n", filePath);
            if (d->d_type == DT_DIR) // 디렉터리라면 내부로 더 탐색
                find_dir(filePath, currentDepth, maxDepth, preg);
        }
    }
    closedir(dp);
}

int main(int argc, char *argv[]) {
    char * pathname = ".";
    char * pattern = "";
    struct stat sb;
    int maxDepth = INT_MAX, opt, currentDepth = 1, enable_pattern = 0;
    regex_t preg;


    // getopt: (-d 같은) 옵션 처리를 도와주는 함수
    // optarg 옵션 인자값
    while ((opt = getopt(argc, argv, "d:n:")) != -1) {
        switch (opt) {
            case 'd':
                maxDepth = atoi(optarg);
                if (maxDepth < 0) {
                    fprintf(stderr, "Max depth must be positive.\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'n':
                pattern = optarg;
                enable_pattern = 1;
                break;
            default:
                break;
        }
    }

    if (argc > 3 && optind == 1) {
        fprintf(stderr, "Usage: %s -d [max depth] -n [pattern] [filepath]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (optind == argc - 1) { // optind: 다음 처리할 argv의 주소
        pathname = argv[optind];
    }

    if (enable_pattern && regcomp(&preg, pattern, 0) != 0)
        handle_error("regcomp");

    if (stat(pathname, &sb) == -1)
        handle_error("stat");

    if (!enable_pattern || (enable_pattern && regexec(&preg, pathname, 0, NULL, 0) != REG_NOMATCH))
        printf("%s\n", pathname);

    if (S_ISDIR(sb.st_mode)) {
        if (enable_pattern)
            find_dir(pathname, currentDepth, maxDepth, &preg);
        else
            find_dir(pathname, currentDepth, maxDepth, NULL);
    }

    exit(EXIT_SUCCESS);
}