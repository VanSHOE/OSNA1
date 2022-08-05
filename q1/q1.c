#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        return;
    }
    char buff[1];
    int input = open(argv[1], O_RDONLY);
    mkdir("./Assignment", 0777);

    int output = open("./Assignment/output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);

    int inputLength = 0;

    while (read(input, buff, 1) > 0)
    {
        inputLength++;
    }

    int pos = -1;
    lseek(input, pos--, SEEK_END);
    int outputLength = 0;
    while (read(input, buff, 1) > 0 && outputLength < inputLength)
    {
        write(output, buff, 1);
        outputLength++;
        double progress = (double)outputLength * 100 / (double)inputLength;
        char progressString[100];
        sprintf(progressString, "\r%.2f%%", progress);
        write(STDOUT_FILENO, progressString, strlen(progressString));
        lseek(input, pos--, SEEK_END);
    }
    write(STDOUT_FILENO, "\n", 1);
    close(input);
    close(output);
}