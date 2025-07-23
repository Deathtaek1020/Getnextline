/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getnextline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deathtaek <deathtaek@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:54:23 by zorazafi          #+#    #+#             */
/*   Updated: 2025/07/23 17:38:53 by deathtaek        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

size_t ft_strlen(const char *str)
{
    size_t i;

    if (!str)
        return (0);
    i = 0;
    while (str[i])
        i++;
    return (i);
}
char *ft_strchr(const char *str, int c)
{
    int i;

    if (!str)
        return (0);
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
char *ft_strdup(const char *str)
{
    char    *temp;
    int     i;
    int     lenght;

    i = 0;
    if (!str)
        return (0);
    lenght = ft_strlen(str);
    temp = malloc((lenght + 1) * sizeof(char));
    if (!temp)
        return (NULL);
    while(str[i])
    {
        temp[i] = str[i];
        i++;
    }
    temp[i] = '\0';
    return (temp);
}

char *ft_substr(char const *s, unsigned int start, size_t len)
{
    char    *temp;
    size_t  i;
    size_t  s_len;

    i = 0;
    if (!s)
        return(NULL);
    s_len = ft_strlen(s);
    if (start >= s_len)
        return (ft_strdup(""));
    if (len > s_len - start)
        len = s_len - start;
    temp = malloc((len + 1) * sizeof(char));
    if (!temp)
        return (NULL);
    while (i < len)
    {
        temp[i] = s[start + i];
        i++;
    }
    temp[i] = '\0';
    return (temp);
}
char *ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	size_t	i;
	size_t	j;
	char	*temp;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2);
	temp = malloc((total_len + 1) * sizeof(char));
	if (!temp)
		return (NULL);
	while (s1[i])
	{
		temp[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		temp[i + j] = s2[j];
		j++;
	}
	temp[i + j] = '\0';
	return (temp);
}


 // ------------------------------------------------------------------------------------------------------------

char	*extract_line(char *stash)
{
    size_t	i;
    char	*line;

    if (!stash || !stash[0])
        return (NULL);
    i = 0;
    while (stash[i] && stash[i] != '\n')
        i++;
    if (stash[i] == '\n')
        i++;
    line = ft_substr(stash, 0, i);
    return (line);
}

char	*clean_stash(char *stash)
{
    size_t	i;
    char	*new_stash;

    if (!stash)
        return (NULL);
    i = 0;
    while (stash[i] && stash[i] != '\n')
        i++;
    if (!stash[i])
    {
        free(stash);
        return (NULL);
    }
    new_stash = ft_strdup(stash + i + 1);
    free(stash);
    return (new_stash);
}

static int	read_to_buffer(int fd, char *buffer, char **stash)
{
    ssize_t	bytes;
    char	*tmp;

    bytes = read(fd, buffer, BUFFER_SIZE);
    if (bytes < 0)
    {
        free(*stash);
        *stash = NULL;
        return (-1);
    }
    buffer[bytes] = '\0';
    tmp = *stash;
    if (tmp)
        *stash = ft_strjoin(tmp, buffer);
    else
        *stash = ft_strdup(buffer);
    if (tmp)
        free(tmp);
    if (!*stash)
        return (-1);
    return (bytes);
}

char	*read_and_stash(int fd, char *stash)
{
    char	*buffer;
    int		bytes;

    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer)
        return (NULL);
    bytes = 1;
    while (!ft_strchr(stash, '\n') && bytes > 0)
    {
        bytes = read_to_buffer(fd, buffer, &stash);
        if (bytes < 0)
        {
            free(buffer);
            return (NULL);
        }
    }
    free(buffer);
    return (stash);
}

char	*get_next_line(int fd)
{
    static char	*stash;
    char		*line;

    if (fd < 0 || BUFFER_SIZE <= 0)
        return (NULL);
    stash = read_and_stash(fd, stash);
    if (!stash)
        return (NULL);
    line = extract_line(stash);
    stash = clean_stash(stash);
    return (line);
}

// -----------------------------------

int	main(void)
{
    int		fd;
    char	*line;

    fd = open("test.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return (1);
    }
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}