#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void printPermissions(struct stat data, char *name)
{
    char permissions[9];
    permissions[0] = data.st_mode & S_IRUSR ? 1 : 0;
    permissions[1] = data.st_mode & S_IWUSR ? 1 : 0;
    permissions[2] = data.st_mode & S_IXUSR ? 1 : 0;

    permissions[3] = data.st_mode & S_IRGRP ? 1 : 0;
    permissions[4] = data.st_mode & S_IWGRP ? 1 : 0;
    permissions[5] = data.st_mode & S_IXGRP ? 1 : 0;

    permissions[6] = data.st_mode & S_IROTH ? 1 : 0;
    permissions[7] = data.st_mode & S_IWOTH ? 1 : 0;
    permissions[8] = data.st_mode & S_IXOTH ? 1 : 0;

    char permissionString[100];
    char stakeHolders[3][10];
    strcpy(stakeHolders[0], "User");
    strcpy(stakeHolders[1], "Group");
    strcpy(stakeHolders[2], "Others");

    char types[3][10];
    strcpy(types[0], "read");
    strcpy(types[1], "write");
    strcpy(types[2], "execute");

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            sprintf(permissionString, "%s has %s permission on %s: %s\n", stakeHolders[i], types[j], name, permissions[i * 3 + j] ? "Yes" : "No");
            write(STDOUT_FILENO, permissionString, strlen(permissionString));
        }
    }
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

    printPermissions(newfileData, argv[1]);
    printPermissions(oldfileData, argv[2]);
    printPermissions(dirData, argv[3]);

    // printf("NFst_mode = %o\n", newfileData.st_mode);
    // printf("OFst_mode = %o\n", oldfileData.st_mode);
    // printf("Dst_mode = %o\n", dirData.st_mode);

    // write(STDOUT_FILENO, "\n", 1);
}