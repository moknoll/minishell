/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hd_is_end(char *line, char *delim)
{
	char	*nl;

	nl = ft_strchr(line, '\n');
	if (nl)
		*nl = '\0';
	if (ft_strcmp(line, delim) == 0)
		return (1);
	return (0);
}

void	hd_write(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static char	*read_line_interactive(void)
{
	char	*line;
	int		capacity = 128;
	int		len = 0;
	int		c;

	line = malloc(capacity);
	if (!line)
		return (NULL);
	
	write(STDOUT_FILENO, "> ", 2);
	while (1)
	{
		if (g_exit_status == 130)
		{
			free(line);
			return (NULL);
		}
		c = getchar();
		if (g_exit_status == 130)
		{
			free(line);
			return (NULL);
		}
		if (c == EOF)
		{
			if (len == 0)
			{
				free(line);
				return (NULL);
			}
			break;
		}
		if (c == '\n')
			break;
		if (len >= capacity - 1)
		{
			capacity *= 2;
			line = realloc(line, capacity);
			if (!line)
				return (NULL);
		}
		
		line[len++] = c;
	}
	line[len] = '\0';
	return (line);
}

static char	*read_line_simple(int fd)
{
	static char	buffer[4096];
	static int	buffer_pos = 0;
	static int	buffer_size = 0;
	char		*line;
	int			line_len;

	if (buffer_size < 0)
	{
		buffer_pos = 0;
		buffer_size = 0;
	}
	line = allocate_line_buffer();
	if (!line)
		return (NULL);
	line_len = 0;
	if (!process_line_reading(fd, buffer, &buffer_pos, &buffer_size, 
			line, &line_len))
	{
		buffer_pos = 0;
		buffer_size = 0;
		free(line);
		return (NULL);
	}
	finalize_line(line, line_len);
	return (line);
}

static int	heredoc_loop(char *delim, int fd)
{
	char	*line;
	int		is_interactive;

	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (g_exit_status == 130)
			return (-1);
		if (is_interactive)
			line = read_line_interactive();
		else
		{
			write(STDOUT_FILENO, "> ", 2);
			line = read_line_simple(STDIN_FILENO);
		}
		if (g_exit_status == 130)
		{
			if (line)
				free(line);
			return (-1);
		}
		if (!line)
			return (1);
		if ((is_interactive && ft_strcmp(line, delim) == 0) ||
			(!is_interactive && hd_is_end(line, delim)))
		{
			free(line);
			break;
		}
		hd_write(fd, line);
		free(line);
	}
	return (0);
}

char	*handle_heredoc(char *delimiter)
{
	char				*tmp_filename;
	int					tmp_fd;
	struct sigaction	old_int;
	int					saved;
	int					ret;

	tmp_fd = create_tmp_heredoc_file(&tmp_filename);
	if (tmp_fd == -1)
		return (NULL);
	setup_heredoc_signals(&old_int, &saved);
	ret = heredoc_loop(delimiter, tmp_fd);
	cleanup_heredoc_signals(&old_int, saved, ret, delimiter);
	close(tmp_fd);
	if (should_cleanup_file(ret))
	{
		unlink(tmp_filename);
		free(tmp_filename);
		return (NULL);
	}
	return (tmp_filename);
}

