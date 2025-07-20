#include <stdio.h>
#include <unistd.h>

void vakio(char *soratra, int limit)
{
    static int i = 0;

    while ((limit > 0) && !(soratra[i] == '\n'))
    {
        write(1, &soratra[i], 1);
        i++;
        limit--;
    }
    if (soratra[i] == '\n')
        i++;
    write(1, "\n", 1);
    return ;
}

int main(void)
{
    char *truc = "ba\nbakoto jhonson ehehehehe";
    vakio(truc, 5);
    vakio(truc, 4);
    vakio(truc, 20);
    vakio(truc, 4);
}