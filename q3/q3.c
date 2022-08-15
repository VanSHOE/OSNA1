#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define buffLen 60000
int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

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
    char buff[buffLen];

    struct stat newfileData;
    struct stat oldfileData;
    struct stat dirData;

    int newFile = open(argv[1], O_RDONLY);
    int oldFile = open(argv[2], O_RDONLY);

    stat(argv[1], &newfileData);
    stat(argv[2], &oldfileData);
    stat(argv[3], &dirData);

    char existence[100];
    sprintf(existence, "Directory is created: %s\n", S_ISDIR(dirData.st_mode) ? "Yes" : "No");
    write(STDOUT_FILENO, existence, strlen(existence));
    char areContentsReversed[100];
    int newLength = 0;
    int curLen = 0;
    while ((curLen = read(newFile, buff, buffLen)) > 0)
    {
        newLength += curLen;
    }

    int oldLength = 0;
    curLen = 0;
    while ((curLen = read(oldFile, buff, buffLen)) > 0)
    {
        oldLength += curLen;
    }

    if (newLength != oldLength)
    {
        sprintf(areContentsReversed, "Whether file contents are reversed in newfile: No\n");
        write(STDOUT_FILENO, areContentsReversed, strlen(areContentsReversed));
    }
    else
    {
        int lengthToGo = newLength;
        lseek(newFile, -min(buffLen, newLength), SEEK_END);
        lseek(oldFile, 0, SEEK_SET);

        int reversed = 1;

        while (lengthToGo)
        {
            char newBuff[buffLen];
            char oldBuff[buffLen];
            int curLenNew = read(newFile, newBuff, buffLen);
            lseek(newFile, -curLenNew, SEEK_CUR);

            int curLenOld = read(oldFile, oldBuff, buffLen);

            curLenNew = min(curLenNew, curLenOld);

            lengthToGo -= curLenNew;

            for (int i = 0; i < curLenNew; i++)
            {
                if (newBuff[i] != oldBuff[curLenNew - i - 1])
                {
                    reversed = 0;
                    break;
                }
            }
            if (!reversed)
            {
                sprintf(areContentsReversed, "Whether file contents are reversed in newfile: No\n");
                write(STDOUT_FILENO, areContentsReversed, strlen(areContentsReversed));
                reversed = 0;
                break;
            }
            lseek(newFile, -min(lengthToGo, buffLen), SEEK_CUR);
        }
        if (reversed)
        {
            sprintf(areContentsReversed, "Whether file contents are reversed in newfile: Yes\n");
            write(STDOUT_FILENO, areContentsReversed, strlen(areContentsReversed));
        }
    }

    printPermissions(newfileData, argv[1]);
    printPermissions(oldfileData, argv[2]);
    printPermissions(dirData, argv[3]);
}