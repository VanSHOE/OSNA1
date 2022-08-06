#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void printPermissions(struct stat data)
{
}

void main(int argc, char **argv)
{
    if (argc != 4)
    {
        char error[100];
        sprintf(error, "Usage: %s <new_file> <old_file> <directory>\n", argv[0]);
        write(STDOUT_FILENO, error, strlen(error));
        return;
    }
    char buff[1];

    struct stat newfileData;
    struct stat oldfileData;
    struct stat dirData;

    int newFile = open(argv[1], O_RDONLY);
    int oldFile = open(argv[2], O_RDONLY);

    stat(argv[1], &newfileData);
    stat(argv[2], &oldfileData);
    stat(argv[3], &dirData);

    printf("NFst_mode = %o\n", newfileData.st_mode);
    printf("OFst_mode = %o\n", oldfileData.st_mode);
    printf("Dst_mode = %o\n", dirData.st_mode);

    // write(STDOUT_FILENO, "\n", 1);
}