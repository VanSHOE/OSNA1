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
    int curLen = 0;
    while ((curLen = read(input, buff, buffLen)) > 0)
    {
        inputLength += curLen;
    }

    if (start < 0 || start >= inputLength)
    {
        char *errorString = "Invalid start index\n";
        write(STDOUT_FILENO, errorString, strlen(errorString));
        return;
    }

    if (end < 0 || end >= inputLength || end < start)
    {
        char *errorString = "Invalid end index\n";
        write(STDOUT_FILENO, errorString, strlen(errorString));
        return;
    }

    int pos = max(0, start - buffLen);
    lseek(input, pos, SEEK_SET);
    int outputLength = 0;
    int endLength = -1;
    while (outputLength < start && (curLen = read(input, buff, buffLen)) > 0)
    {
        lseek(input, -curLen, SEEK_CUR);
        int readLength = endLength != -1 ? endLength : min(curLen, start - pos);
        // reverse buff
        for (int i = 0; i < readLength / 2; i++)
        {
            char temp = buff[i];
            buff[i] = buff[readLength - i - 1];
            buff[readLength - i - 1] = temp;
        }
        write(output, buff, readLength);
        outputLength += readLength;

        double progress = (double)outputLength * 100 / (double)inputLength;
        char progressString[100];
        sprintf(progressString, "\r%.2f%%", progress);
        write(STDOUT_FILENO, progressString, strlen(progressString));

        if (buffLen <= pos)
        {
            lseek(input, -buffLen, SEEK_CUR);
            pos -= buffLen;
        }
        else
        {
            lseek(input, 0, SEEK_SET);
            endLength = pos;
            pos = 0;
        }
    }

    endLength = -1;

    pos = start;
    lseek(input, pos, SEEK_SET);
    while (outputLength < end + 1 && (curLen = read(input, buff, buffLen)) > 0)
    {
        int readLength = min(curLen, end - pos + 1);
        write(output, buff, readLength);
        outputLength += readLength;

        double progress = (double)outputLength * 100 / (double)inputLength;
        char progressString[100];
        sprintf(progressString, "\r%.2f%%", progress);
        write(STDOUT_FILENO, progressString, strlen(progressString));
    }

    pos = max(end + 1, inputLength - buffLen);
    lseek(input, pos, SEEK_SET);
    endLength = -1;
    while ((curLen = read(input, buff, buffLen)) > 0 && outputLength < inputLength)
    {
        curLen = endLength != -1 ? endLength : curLen;
        lseek(input, -curLen, SEEK_CUR);
        // reverse buff
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

        // pos - buffLen >= end + 1

        if (buffLen <= pos - end - 1)
        {
            lseek(input, -buffLen, SEEK_CUR);
            pos -= buffLen;
        }
        else
        {
            lseek(input, end + 1, SEEK_SET);
            endLength = pos - end - 1;
            pos = end + 1;
        }
    }

    write(STDOUT_FILENO, "\n", 1);
    close(input);
    close(output);
}