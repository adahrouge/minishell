/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:10:25 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/14 15:20:46 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define BUFFER_SIZE 42

char	*ft_remove_line(char *static_line)
{
	char	*update_static;
	int		i;

	i = 0;
	if (!static_line)
		return (NULL);
	while (static_line[i] != '\n' && static_line[i] != '\0')
		i++;
	if (static_line[i] == '\0')
	{
		free(static_line);
		return (NULL);
	}
	update_static = ft_substr(static_line, i + 1,
			ft_strlen(static_line) - i - 1);
	if (update_static == NULL)
		return (NULL);
	free(static_line);
	return (update_static);
}

char	*ft_get_target(char *static_line)
{
	char	*target_line;
	int		i;

	if (!static_line)
		return (NULL);
	i = 0;
	while (static_line[i] != '\n' && static_line[i] != '\0')
		i++;
	target_line = ft_substr(static_line, 0, i + 1);
	return (target_line);
}

char	*allocate_and_read_buffer(int fd, int *bytes_read)
{
	char	*buffer;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	*bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (*bytes_read < 0)
	{
		free(buffer);
		return (NULL);
	}
	buffer[*bytes_read] = '\0';
	return (buffer);
}

char	*ft_update_static(char *static_line, int fd)
{
	int		bytes_read;
	char	*buffer;
	char	*tmp;

	while (1)
	{
		buffer = allocate_and_read_buffer(fd, &bytes_read);
		if (!buffer)
			return (NULL);
		if (!static_line)
			tmp = ft_strjoin("", buffer);
		else
		{
			tmp = ft_strjoin(static_line, buffer);
			free(static_line);
		}
		free(buffer);
		if (!tmp)
			return (NULL);
		static_line = tmp;
		if (ft_strchr(static_line, '\n') || bytes_read == 0)
			break ;
	}
	return (static_line);
}

char	*get_next_line(int fd)
{
	static char	*static_line;
	char		*target_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	static_line = ft_update_static(static_line, fd);
	if (!static_line)
	{
		return (NULL);
	}
	target_line = ft_get_target(static_line);
	static_line = ft_remove_line(static_line);
	if (!target_line || !*target_line)
	{
		free(target_line);
		return (NULL);
	}
	return (target_line);
}
