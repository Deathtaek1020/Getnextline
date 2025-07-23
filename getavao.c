/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Deathtaek1020 <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:35:09 by Deathtaek1020    #+#    #+#             */
/*   Updated: 2025/07/23 15:35:09 by Deathtaek1020   ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

/* ========== GET_NEXT_LINE.H ========== */

char	*get_next_line(int fd);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);

/* ========== GET_NEXT_LINE_UTILS.C ========== */

size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *str, int c)
{
	if (!str)
		return (NULL);
	while (*str)
	{
		if (*str == (char)c)
			return ((char *)str);
		str++;
	}
	if ((char)c == '\0')
		return ((char *)str);
	return (NULL);
}

char	*ft_strdup(const char *s1)
{
	char	*dup;
	size_t	i;
	size_t	len;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	dup = malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}



char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	size_t	i;
	size_t	total_len;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	total_len = ft_strlen(s1) + ft_strlen(s2);
	joined = malloc(sizeof(char) * (total_len + 1));
	if (!joined)
		return (NULL);
	i = 0;
	while (*s1)
		joined[i++] = *s1++;
	while (*s2)
		joined[i++] = *s2++;
	joined[i] = '\0';
	return (joined);
}

/* ========== GET_NEXT_LINE.C ========== */

static char	*extract_line(char *buffer)
{
	char	*line;
	size_t	i;
	size_t	len;

	if (!buffer || !*buffer)
		return (NULL);
	len = 0;
	while (buffer[len] && buffer[len] != '\n')
		len++;
	if (buffer[len] == '\n')
		len++;
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = buffer[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}


static void	copy_after_newline(char *dst, char *src, size_t start)
{
	size_t j = 0;
	while (src[start])
	{
		dst[j] = src[start];
		j++;
		start++;
	}
	dst[j] = '\0';
}

static char	*update_buffer(char *buffer)
{
	char	*new_buffer;
	size_t	i;
	size_t	len;

	if (!buffer)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	i++;
	len = ft_strlen(buffer) - i;
	new_buffer = malloc(sizeof(char) * (len + 1));
	if (!new_buffer)
		return (NULL);
	copy_after_newline(new_buffer, buffer, i);
	free(buffer);
	return (new_buffer);
}

static char	*read_chunk(int fd, char *buffer, int *bytes_read)
{
	char	*temp_buffer;
	char	*temp;

	temp_buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!temp_buffer)
		return (NULL);
	*bytes_read = read(fd, temp_buffer, BUFFER_SIZE);
	if (*bytes_read == -1)
	{
		free(temp_buffer);
		return (NULL);
	}
	temp_buffer[*bytes_read] = '\0';
	temp = ft_strjoin(buffer, temp_buffer);
	free(temp_buffer);
	free(buffer);
	return (temp);
}

static char	*read_and_join(int fd, char *buffer)
{
	int	bytes_read;

	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(buffer, '\n'))
	{
		buffer = read_chunk(fd, buffer, &bytes_read);
		if (!buffer)
			return (NULL);
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_and_join(fd, buffer);
	if (!buffer)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	line = extract_line(buffer);
	buffer = update_buffer(buffer);
	return (line);
}

/* ========== MAIN DE TEST ========== */

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