/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 14:03:11 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*append_read_line(char *line, int *len, int *capacity, int c)
{
	char	*new_line;

	if (*len >= *capacity - 1)
	{
		*capacity *= 2;
		new_line = ft_realloc(line, *capacity);
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
		c = ft_getchar();
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

int	handle_heredoc(char *delimiter)
{
	int					pipe_fd[2];
	pid_t				pid;
	struct sigaction	old_int;
	int					saved;
	int					ret;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	setup_heredoc_signals(&old_int, &saved);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		ret = heredoc_loop(delimiter, pipe_fd[1]);
		close(pipe_fd[1]);
		cleanup_heredoc_signals(&old_int, saved);
		exit(ret == -1 ? 130 : 0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &ret, 0);
	cleanup_heredoc_signals(&old_int, saved);
	if (WEXITSTATUS(ret) == 130)
	{
		close(pipe_fd[0]);
		g_exit_status = 130;
		return (-1);
	}
	return (pipe_fd[0]);
}
