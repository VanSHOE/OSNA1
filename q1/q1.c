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

void main(int argc, char **argv)
{
    if (argc != 2)
    {
        char error[100];
        sprintf(error, "Usage: %s <input_file>\n", argv[0]);
        write(STDOUT_FILENO, error, strlen(error));
        return;
    }
    char buff[buffLen];
    int input = open(argv[1], O_RDONLY);
    mkdir("./Assignment", 0777);
    const char *outputPath = "./Assignment/1_";
    char outputFileName[strlen(argv[1]) + strlen(outputPath) + 1];
    strcpy(outputFileName, outputPath);
    strcat(outputFileName, argv[1]);

    int output = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    int inputLength = 0;
    int curLen = 0;
    while ((curLen = read(input, buff, buffLen)) > 0)
    {
        inputLength += curLen;
    }
    // printf("%d\n",inputLength);

    lseek(input, -min(buffLen, inputLength), SEEK_END);
    int outputLength = 0;
    int endLength = -1;
    while ((curLen = read(input, buff, buffLen)) > 0 && outputLength < inputLength)
    {
        curLen = endLength == -1 ? curLen : endLength;
        lseek(input, -curLen, SEEK_CUR);
        // printf("\nWriting %d bytes starting with %c\n", curLen, buff[0]);
        for (int i = 0; i < curLen / 2; i++)
        {
            char temp = buff[i];
            buff[i] = buff[curLen - i - 1];
            buff[curLen - i - 1] = temp;
        }

        write(output, buff, curLen);
        outputLength += curLen;

        double progress = (double)outputLength * 100 / (double)inputLength;
        char progressString[100];
        sprintf(progressString, "\r%.2f%%", progress);
        write(STDOUT_FILENO, progressString, strlen(progressString));

        // lseek(input, -min(buffLen, inputLength - outputLength), SEEK_CUR);
        if (buffLen <= inputLength - outputLength)
        {
            lseek(input, -buffLen, SEEK_CUR);
        }
        else
        {
            lseek(input, -(inputLength - outputLength), SEEK_CUR);
            endLength = inputLength - outputLength;
        }
    }
    write(STDOUT_FILENO, "\n", 1);
    close(input);
    close(output);
}