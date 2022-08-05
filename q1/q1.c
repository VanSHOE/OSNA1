#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

void main()
{
    lseek(0, 0, 0);
}