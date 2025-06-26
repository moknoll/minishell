/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:29:50 by radubos           #+#    #+#             */
/*   Updated: 2025/06/26 00:12:09 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_infile(t_redir *redir, int *fd, int *std_fd)
{
	*fd = open(redir->file, O_RDONLY);
	*std_fd = STDIN_FILENO;
	return (0);
}

static int	open_outfile(t_redir *redir, int *fd, int *std_fd)
{
	*fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	*std_fd = STDOUT_FILENO;
	return (0);
}

static int	open_appendfile(t_redir *redir, int *fd, int *std_fd)
{
	*fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	*std_fd = STDOUT_FILENO;
	return (0);
}

static int	open_heredoc(t_redir *redir, int *fd, int *std_fd)
{
	if (redir->fd == -1)
	{
		printf("minishell: heredoc pipe not set\n");
		return (-1);
	}
	*fd = redir->fd;
	*std_fd = STDIN_FILENO;
	return (0);
}

int	open_redir_file(t_redir *redir, int *fd, int *std_fd)
{
	if (redir->type == REDIR_IN)
		return (open_infile(redir, fd, std_fd));
	else if (redir->type == REDIR_OUT)
		return (open_outfile(redir, fd, std_fd));
	else if (redir->type == REDIR_APPEND)
		return (open_appendfile(redir, fd, std_fd));
	else if (redir->type == REDIR_HEREDOC)
		return (open_heredoc(redir, fd, std_fd));
	printf("minishell: unknown redirection type\n");
	return (-1);
}
