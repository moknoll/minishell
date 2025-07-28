/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/28 11:36:15 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_read_line(char *line, int *len, int *capacity, int c)
{
	char	*new_line;

	if (*len >= *capacity - 1)
	{
		*capacity *= 2;
		new_line = realloc(line, *capacity);
		if (!new_line)
		{
			free(line);
			return (NULL);
		}
		line = new_line;
	}
	line[(*len)++] = c;
	return (line);
}

static char	*read_line_loop(char *line, int capacity)
{
	int		len;
	int		c;

	len = 0;
	while (1)
	{
		if (g_exit_status == 130)
			return (free(line), NULL);
		c = getchar();
		if (g_exit_status == 130)
			return (free(line), NULL);
		if (c == EOF)
		{
			if (len == 0)
				return (free(line), NULL);
			break ;
		}
		if (c == '\n')
			break ;
		line = append_read_line(line, &len, &capacity, c);
		if (!line)
			return (NULL);
	}
	line[len] = '\0';
	return (line);
}

char	*read_line_interactive(void)
{
	char	*line;
	int		capacity;

	capacity = 128;
	line = malloc(capacity);
	if (!line)
		return (NULL);
	write(STDOUT_FILENO, "> ", 2);
	return (read_line_loop(line, capacity));
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
		line = heredoc_read_line(is_interactive);
		if (g_exit_status == 130)
		{
			free(line);
			return (-1);
		}
		if (!line)
			return (1);
		if (heredoc_is_delim(line, delim, is_interactive))
		{
			free(line);
			break ;
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
