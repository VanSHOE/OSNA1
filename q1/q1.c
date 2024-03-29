#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define buffLen 60000

long long min(long long a, long long b)
{
    return a < b ? a : b;
}

long long max(long long a, long long b)
{
    return a > b ? a : b;
}

void main(long long argc, char **argv)
{
    if (argc != 2)
    {
        char error[100];
        sprintf(error, "Usage: %s <input_file>\n", argv[0]);
        write(STDOUT_FILENO, error, strlen(error));
        return;
    }
    char buff[buffLen];
    long long input = open(argv[1], O_RDONLY);
    mkdir("./Assignment", 0777);
    const char *outputPath = "./Assignment/1_";

    char *fileName = argv[1];
    // Find the file name
    for (long long i = 0; i < strlen(argv[1]) - 1; i++)
    {
        if (argv[1][i] == '/')
        {
            fileName = &argv[1][i + 1];
        }
    }

    char outputFileName[strlen(fileName) + strlen(outputPath) + 1];
    strcpy(outputFileName, outputPath);
    strcat(outputFileName, fileName);

    long long output = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    long long inputLength = 0;
    long long curLen = 0;
    // Iterate through the input file and find the length of the input file
    while ((curLen = read(input, buff, buffLen)) > 0)
    {
        inputLength += curLen;
    }

    // Start the cursor from the end so the buffer reading ends at the last item
    lseek(input, -min(buffLen, inputLength), SEEK_END);
    long long outputLength = 0;
    long long endLength = -1; // This variable will be changed from -1 when the beginning of file is reached where the data to be read can be less than buffLen
    while ((curLen = read(input, buff, buffLen)) > 0 && outputLength < inputLength)
    {
        // length to be read depends on the bytes received, if the beginning of file is reached, the data to be read can be less than buffLen
        curLen = endLength == -1 ? curLen : endLength;
        lseek(input, -curLen, SEEK_CUR);

        // reverse buffer
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