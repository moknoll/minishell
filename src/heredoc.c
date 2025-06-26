/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:46:27 by radubos           #+#    #+#             */
/*   Updated: 2025/06/25 22:52:31 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_heredoc(t_redir *r)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("minishell: heredoc pipe");
		exit(1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strcmp(line, r->file) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	r->fd = pipefd[0];
}

void	handle_heredocs(t_command *cmd_list)
{
	t_command	*cmd;
	t_redir		*r;

	setup_heredoc_signals();
	cmd = cmd_list;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == REDIR_HEREDOC)
				process_heredoc(r);
			r = r->next;
		}
		cmd = cmd->next;
	}
	setup_parent_signals();
}
