#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PATH>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct statx st;
    if (statx(AT_FDCWD, argv[1], 0, STATX_BTIME, &st) == -1) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    // 少なくともXFSでは, ファイル内容を変更した時にbtimeが1変更されるため注意.
    time_t tv_sec = (time_t)st.stx_btime.tv_sec;
    printf("btime: %s", ctime(&tv_sec));

    exit(EXIT_SUCCESS);
}
