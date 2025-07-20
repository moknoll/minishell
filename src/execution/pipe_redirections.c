/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_redirection_args(char **cmd, int i)
{
	while (cmd[i + 2])
	{
		cmd[i] = cmd[i + 2];
		i++;
	}
	cmd[i] = NULL;
	cmd[i + 1] = NULL;
}

int	handle_output_redirection(char **cmd, int i)
{
	char	*filename;
	int		fd;

	filename = cmd[i + 1];
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	remove_redirection_args(cmd, i);
	return (1);
}

int	handle_append_redirection(char **cmd, int i)
{
	char	*filename;
	int		fd;

	filename = cmd[i + 1];
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	remove_redirection_args(cmd, i);
	return (1);
}

int	handle_input_redirection(char **cmd, int i)
{
	char	*filename;
	int		fd;

	filename = cmd[i + 1];
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	remove_redirection_args(cmd, i);
	return (1);
}
