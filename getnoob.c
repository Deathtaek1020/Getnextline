#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000

int main(void)
{
    int     fd;
    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    fd = open("test.txt", O_RDONLY);
    if (fd == -1)
        return(1);
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        write(1, buffer, bytes_read);
    }
    if (bytes_read == -1)
        write(2, "error", 5);
    close(fd);
    return(0);
}