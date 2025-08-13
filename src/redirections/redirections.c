/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 14:06:18 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_heredoc_redirection(char **cmd, int i)
{
	char	*heredoc_file;
	int		saved_stdin;

	if (!validate_cmd_redirection_args(cmd, i))
		return (0);
	heredoc_file = handle_heredoc(cmd[i + 1]);
	if (!heredoc_file)
	{
		g_exit_status = 130;
		return (0);
	}
	if (setup_heredoc_fd(heredoc_file, &saved_stdin) == -1)
		return (0);
	remove_redirection_args(cmd, i);
	close(saved_stdin);
	free(heredoc_file);
	return (1);
}

int	handle_output_redirection(char **cmd, int i, int append)
{
	int	fd;

	if (!validate_cmd_redirection_args(cmd, i))
		return (0);
	fd = open_output_file(cmd[i + 1], append);
	if (fd == -1)
	{
		perror(cmd[i + 1]);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	remove_redirection_args(cmd, i);
	return (1);
}

int	handle_input_redirection(char **cmd, int i)
{
	int	fd;

	if (!validate_cmd_redirection_args(cmd, i))
		return (0);
	fd = open(cmd[i + 1], O_RDONLY);
	if (fd == -1)
	{
		perror(cmd[i + 1]);
		g_exit_status = 1;
		return (0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	remove_redirection_args(cmd, i);
	return (1);
}

int	process_redirections(char **cmd)
{
	int	i;
	int	result;

	i = 0;
	while (cmd[i])
	{
		result = process_cmd_redirections(cmd, i);
		if (result == 0)
			return (0);
		if (result == 1)
			i = 0;
		else
			i++;
	}
	return (1);
}
