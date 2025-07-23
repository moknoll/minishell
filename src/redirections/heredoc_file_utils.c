/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritz <moritz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/23 12:32:34 by moritz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_tmp_heredoc_file(char **filename)
{
	*filename = heredoc_tmp();
	if (!*filename)
		return (-1);
	return (open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0600));
}

int	should_cleanup_file(int ret)
{
	return (ret == -1 || g_exit_status == 130);
}

void	setup_heredoc_signals(struct sigaction *old_int, int *saved)
{
	*saved = g_exit_status;
	disable_echoctl();
	hd_set_signals(old_int);
	g_exit_status = 0;
}

void	cleanup_heredoc_signals(const struct sigaction *old_int, 
		int saved, int ret, char *delimiter)
{
	(void)ret;
	(void)delimiter;
	enable_echoctl();
	hd_restore_signals(old_int);
	if (g_exit_status != 130)
		g_exit_status = saved;
}

void	hd_write(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}