#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int power(int base, int exponent)
{
    int result = 1;
    for (int i = 0; i < exponent; i++)
    {
        result *= base;
    }
    return result;
}

int convertStringtoNumber(char *string)
{
    int number = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        number += (string[i] - '0') * power(10, strlen(string) - i - 1);
    }

    return number;
}

void main(int argc, char **argv)
{
    if (argc != 4)
    {
        char error[100];
        sprintf(error, "Usage: %s <input_file> start_char end_char\n", argv[0]);
        write(STDOUT_FILENO, error, strlen(error));
        return;
    }
    char buff[1];
    int start = convertStringtoNumber(argv[2]);
    int end = convertStringtoNumber(argv[3]);
    int input = open(argv[1], O_RDONLY);
    mkdir("./Assignment", 0777);
    const char *outputPath = "./Assignment/2_";
    char outputFileName[strlen(argv[1]) + strlen(outputPath) + 1];
    strcpy(outputFileName, outputPath);
    strcat(outputFileName, argv[1]);

    int output = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    int inputLength = 0;

    while (read(input, buff, 1) > 0)
    {
        inputLength++;
    }

    if (start < 0 || start >= inputLength)
    {
        char *errorString = "Invalid start index\n";
        write(STDOUT_FILENO, errorString, strlen(errorString));
        return;
    }

    if (end < 0 || end >= inputLength)
    {
        char *errorString = "Invalid end index\n";
        write(STDOUT_FILENO, errorString, strlen(errorString));
        return;
    }

    int pos = start - 1;
    lseek(input, pos--, SEEK_SET);
    int outputLength = 0;
    while (outputLength < start && read(input, buff, 1) > 0)
    {
        write(output, buff, 1);
        outputLength++;
        double progress = (double)outputLength * 100 / (double)inputLength;
        char progressString[100];
        sprintf(progressString, "\r%.2f%%", progress);
        write(STDOUT_FILENO, progressString, strlen(progressString));
        lseek(input, pos--, SEEK_SET);
    }

    lseek(input, start, SEEK_SET);
    while (outputLength < end + 1 && read(input, buff, 1) > 0)
    {
        write(output, buff, 1);
        outputLength++;
        double progress = (double)outputLength * 100 / (double)inputLength;
        char progressString[100];
        sprintf(progressString, "\r%.2f%%", progress);
        write(STDOUT_FILENO, progressString, strlen(progressString));
    }

    pos = -1;
    lseek(input, pos--, SEEK_END);
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