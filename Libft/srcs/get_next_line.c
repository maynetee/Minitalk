/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 17:10:07 by mteichma          #+#    #+#             */
/*   Updated: 2025/02/01 17:10:16 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	handle_join(char **buf, char *tmp)
{
	char	*j;

	j = ft_strjoin(*buf, tmp);
	free(tmp);
	if (!j)
	{
		free(*buf);
		*buf = NULL;
		return (-1);
	}
	free(*buf);
	*buf = j;
	return (1);
}

static int	read_to_buffer(int fd, char **buf)
{
	char	*tmp;
	ssize_t	r;

	tmp = malloc(BUFFER_SIZE + 1);
	if (!tmp)
		return (-1);
	r = read(fd, tmp, BUFFER_SIZE);
	if (r <= 0)
	{
		free(tmp);
		return (r);
	}
	tmp[r] = '\0';
	if (!*buf)
	{
		*buf = ft_strdup(tmp);
		free(tmp);
		if (!*buf)
			return (-1);
		return (1);
	}
	return (handle_join(buf, tmp));
}

static void	clean_buffer(char **buf)
{
	if (!*buf || **buf == '\0')
	{
		free(*buf);
		*buf = NULL;
	}
}

static char	*extract_line(char **buf)
{
	size_t	i;
	int		extra;
	char	*out;
	char	*tmp;

	if (!*buf || !**buf)
		return (NULL);
	i = 0;
	while ((*buf)[i] && (*buf)[i] != '\n')
		i++;
	extra = 0;
	if ((*buf)[i] == '\n')
		extra = 1;
	out = malloc((i + 1 + extra) * sizeof(char));
	if (!out)
		return (NULL);
	ft_memcpy(out, *buf, i);
	if (extra)
		out[i++] = '\n';
	out[i] = '\0';
	tmp = ft_strdup(&(*buf)[i]);
	free(*buf);
	*buf = tmp;
	clean_buffer(buf);
	return (out);
}

char	*get_next_line(int fd)
{
	static char	*buffers[1024];
	char		*out;
	int			rs;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)
		return (NULL);
	while (buffers[fd] == NULL || !ft_strchr(buffers[fd], '\n'))
	{
		rs = read_to_buffer(fd, &buffers[fd]);
		if (rs == -1 || (rs <= 0 && !buffers[fd]))
		{
			free(buffers[fd]);
			buffers[fd] = NULL;
			return (NULL);
		}
		if (rs <= 0)
			break ;
	}
	out = extract_line(&buffers[fd]);
	if (!out)
	{
		free(buffers[fd]);
		buffers[fd] = NULL;
	}
	return (out);
}
