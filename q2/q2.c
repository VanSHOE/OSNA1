#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define buffLen 60000

long long min(long long a, long long b)
{
    return a < b ? a : b;
}

long long max(long long a, long long b)
{
    return a > b ? a : b;
}

long long power(long long base, long long exponent)
{
    long long result = 1;
    for (long long i = 0; i < exponent; i++)
    {
        result *= base;
    }
    return result;
}

long long convertStringtoNumber(char *string)
{
    long long number = 0;
    for (long long i = 0; i < strlen(string); i++)
    {
        number += (string[i] - '0') * power(10, strlen(string) - i - 1);
    }

    return number;
}

void main(long long argc, char **argv)
{
    if (argc != 4)
    {
        char error[100];
        sprintf(error, "Usage: %s <input_file> start_char end_char\n", argv[0]);
        write(STDOUT_FILENO, error, strlen(error));
        return;
    }
    char buff[buffLen];
    long long start = atoi(argv[2]);
    long long end = atoi(argv[3]);
    // printf arg1
    // printf("%s\n", argv[1]);
    long long input = open(argv[1], O_RDONLY);
    mkdir("./Assignment", 0777);
    const char *outputPath = "./Assignment/2_";

    // get file name from input path with loop
    char *fileName = argv[1];
    for (long long i = 0; i < strlen(argv[1]) - 1; i++)
    {
        if (argv[1][i] == '/')
        {
            fileName = &argv[1][i + 1];
        }
    }
    // printf file name
    // printf("%s\n", fileName);
    char outputFileName[strlen(fileName) + strlen(outputPath) + 1];
    strcpy(outputFileName, outputPath);
    strcat(outputFileName, fileName);
    // printf path
    // printf("%s\n", outputFileName);

    long long output = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    long long inputLength = 0;
    long long curLen = 0;
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

    long long pos = max(0, start - buffLen);
    lseek(input, pos, SEEK_SET);
    long long outputLength = 0;
    long long endLength = -1;
    while (outputLength < start && (curLen = read(input, buff, buffLen)) > 0)
    {
        lseek(input, -curLen, SEEK_CUR);
        long long readLength = endLength != -1 ? endLength : min(curLen, start - pos);
        // reverse buff
        for (long long i = 0; i < readLength / 2; i++)
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
        lseek(input, -curLen, SEEK_CUR);

        long long readLength = min(curLen, end - pos + 1);
        write(output, buff, readLength);
        outputLength += readLength;

        lseek(input, readLength, SEEK_CUR);
        pos += readLength;

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
        for (long long i = 0; i < curLen / 2; i++)
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
        // printf("At end2\n");

        // pos - buffLen >= end + 1
        // printf("%lld\n", pos);
        if (buffLen <= pos - end - 1)
        {
            lseek(input, -buffLen, SEEK_CUR);
            pos -= buffLen;
        }
        else
        {
            // printf("At end\n");
            lseek(input, end + 1, SEEK_SET);
            endLength = pos - end - 1;
            pos = end + 1;
        }
    }

    write(STDOUT_FILENO, "\n", 1);
    // printf("At endfinal\n");
    close(input);
    close(output);
    // printf("At endfinal22\n");
}