/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:29:50 by radubos           #+#    #+#             */
/*   Updated: 2025/06/27 11:41:13 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_type_to_fd(int type)
{
	if (type == REDIR_IN)
		return (STDIN_FILENO);
	else if (type == REDIR_HEREDOC)
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

int	open_redirection_file(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (redir->type == REDIR_HEREDOC)
		fd = redir->fd;
	return (fd);
}

int	redirect_fd(int from_fd, int to_fd)
{
	if (dup2(from_fd, to_fd) == -1)
	{
		perror("minishell: dup2");
		close(from_fd);
		return (-1);
	}
	close(from_fd);
	return (0);
}

int	apply_redirections(t_redir *redirs)
{
	int	fd;
	int	target_fd;

	while (redirs)
	{
		target_fd = redir_type_to_fd(redirs->type);
		fd = open_redirection_file(redirs);
		if (fd < 0)
		{
			fprintf(stderr, "minishell: %s: %s\n",
				redirs->file, strerror(errno));
			return (-1);
		}
		if (redirect_fd(fd, target_fd) == -1)
			return (-1);
		redirs = redirs->next;
	}
	return (0);
}
