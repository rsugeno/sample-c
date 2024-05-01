#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PATH>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat st;
    if (lstat(argv[1], &st) == -1) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    printf("uid: %d\n", st.st_uid);
    printf("gid: %d\n", st.st_gid);
    printf("size: %ld\n", st.st_size);
    printf("atime: %s", ctime(&st.st_atime));
    printf("mtime: %s", ctime(&st.st_mtime));
    printf("ctime: %s", ctime(&st.st_ctime));

    exit(EXIT_SUCCESS);
}
