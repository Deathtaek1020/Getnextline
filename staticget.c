#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFFER_SIZE 1000

int read_next_char(int fd, char *c)
{
    static char buffer[BUFFER_SIZE];
    static int  buffer_pos = 0;
    static int  bytes_in_buffer = 0;

    if (buffer_pos >= bytes_in_buffer)
    {
        bytes_in_buffer = read(fd, buffer, BUFFER_SIZE);
        buffer_pos = 0;
        if (bytes_in_buffer <= 0)
            return (bytes_in_buffer);
    }
    *c = buffer[buffer_pos];
    buffer_pos++;
    return (1);
}

int main(void)
{
    int     fd;
    char    c;
    int     result;

    fd = open("test.txt", O_RDONLY);
    if (fd == -1)
        return (1);
    while ((result = read_next_char(fd, &c)) > 0)
    {
        write(1, &c, 1);
    }
    return(0);
}