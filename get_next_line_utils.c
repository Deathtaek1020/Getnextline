#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

size_t ft_strlen(const char *str)
{
    size_t i;

    i = 0;
    while (str[i])
    {
        i++;
    }
    return (i);
}
char *ft_strchr(const char *str, int c)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == (char)(c))
            return ((char *)&str[i]);
        i++;
    }
    if (str[i] == (char)(c))
        return ((char *)&str[i]);
    return (NULL);
}

// int main(void)
// {
//     char *test = "Hello World";
//     char *result;

//     result = ft_strchr(test, 'o');
//     if (result)
//         printf("'o' trouvé à la position %ld\n", result - test);
//     else
//         printf("'o' non trouvé\n");
// }

char *ft_strdup(const char *str)
{
    char    *temp;
    int     i;
    int     lenght;

    lenght = ft_strlen(str);
    i = 0;
    temp = malloc((lenght + 1) * sizeof(char));
    if (!temp)
        return (NULL);
    while(str[i])
    {
        temp[i] = str[i];
        i++;
    }
    temp[i] = str[i];
    return (temp);
}

// int main(void)
// {
//     char *one = "hehehehe";
//     char *two = ft_strdup(one);

//     printf("le truc est : %s", two);
// }

int main()
{
    printf("Longueur de 'Hello': %zu\n", ft_strlen("Hello"));
    
    char *found = ft_strchr("Hello World", 'W');
    if (found)
        printf("Trouvé 'W' à: %s\n", found);
    
    char *copy = ft_strdup("Test");
    if (copy) {
        printf("Copie: %s\n", copy);
        free(copy);
    }
    
    return 0;
}