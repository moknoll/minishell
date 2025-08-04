/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 12:31:25 by moritz            #+#    #+#             */
/*   Updated: 2025/08/04 14:25:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

char	*read_line_simple(int fd)
{
	static char				buffer[4096];
	static t_buffer_state	state;
	t_line_state			line;

	if (state.size < 0)
	{
		state.pos = 0;
		state.size = 0;
	}
	line.data = allocate_line_buffer();
	if (!line.data)
		return (NULL);
	return (read_line_or_cleanup(fd, buffer, &state, &line));
}

char	*heredoc_read_line(int is_interactive)
{
	char	*line;

	if (is_interactive)
		line = read_line_interactive();
	else
	{
		write(STDOUT_FILENO, "> ", 2);
		line = read_line_simple(STDIN_FILENO);
	}
	return (line);
}

int	heredoc_is_delim(char *line, char *delim, int is_interactive)
{
	if (!line)
		return (0);
	if (is_interactive)
		return (ft_strcmp(line, delim) == 0);
	else
		return (hd_is_end(line, delim));
}

char	*read_line_or_cleanup(int fd, char *buffer,
	t_buffer_state *state, t_line_state *line)
{
	line->len = 0;
	if (!process_line_reading(fd, buffer, state, line))
	{
		state->pos = 0;
		state->size = 0;
		free(line->data);
		return (NULL);
	}
	finalize_line(line->data, line->len);
	return (line->data);
}
