/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 14:01:16 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	setup_heredoc_signals(struct sigaction *old_int, int *saved)
{
	*saved = g_exit_status;
	disable_echoctl();
	hd_set_signals(old_int);
	g_exit_status = 0;
}

void	cleanup_heredoc_signals(const struct sigaction *old_int,
		int saved)
{
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
