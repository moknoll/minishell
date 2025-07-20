/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fill_buffer(int fd, char *buffer, int *buffer_pos, int *buffer_size)
{
	if (*buffer_pos >= *buffer_size)
	{
		*buffer_size = read(fd, buffer, 4096);
		if (*buffer_size <= 0)
			return (0);
		*buffer_pos = 0;
	}
	return (1);
}

int	read_character_to_line(char *buffer, int *buffer_pos, 
		char *line, int *line_len)
{
	if (buffer[*buffer_pos] == '\n')
	{
		(*buffer_pos)++;
		return (1);
	}
	line[(*line_len)++] = buffer[(*buffer_pos)++];
	if (*line_len >= 1023)
		return (1);
	return (0);
}

void	finalize_line(char *line, int line_len)
{
	if (line_len == 0)
	{
		line[0] = '\n';
		line[1] = '\0';
	}
	else
	{
		line[line_len] = '\n';
		line[line_len + 1] = '\0';
	}
}

char	*allocate_line_buffer(void)
{
	char	*line;

	line = malloc(1024);
	return (line);
}

int	process_line_reading(int fd, char *buffer, int *buffer_pos, 
		int *buffer_size, char *line, int *line_len)
{
	while (1)
	{
		if (!fill_buffer(fd, buffer, buffer_pos, buffer_size))
		{
			if (*line_len == 0)
				return (0);
			break;
		}
		if (read_character_to_line(buffer, buffer_pos, line, line_len))
			break;
	}
	return (1);
}
