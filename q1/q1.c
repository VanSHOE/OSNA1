#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

void main()
{
    char buff[1];
    int input = open("input.txt", O_RDONLY);
    int output = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);

    int inputLength = 0;

    while (read(input, buff, 1) > 0)
    {
        inputLength++;
    }
    // Reverse file
    int pos = -1;
    lseek(input, pos--, SEEK_END);
    int outputLength = 0;
    while (read(input, buff, 1) > 0 && outputLength < inputLength)
    {
        write(output, buff, 1);
        outputLength++;
        lseek(input, pos--, SEEK_END);
    }

    close(input);
    close(output);
}