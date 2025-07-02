/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:46:27 by radubos           #+#    #+#             */
/*   Updated: 2025/06/28 09:02:05 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc_forked(t_redir *r)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), -1);

	pid = fork();
	if (pid == -1)
		return (perror("minishell: fork"), close(pipefd[0]), close(pipefd[1]), -1);

	if (pid == 0)
	{
		setup_heredoc_signals();
		close(pipefd[0]);
		char *line;
		while (1)
		{
			line = readline("heredoc> ");
			if (!line)
				exit(130);
			if (ft_strcmp(line, r->file) == 0)
			{
				free(line);
				break;
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		exit(0);
		}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		r->fd = -1;
		g_exit_status = 130;
		return (-1);
	}
	r->fd = pipefd[0];
	return (0);
}

void	handle_heredocs(t_command *cmd_list)
{
	t_command	*cmd;
	t_redir		*r;

	cmd = cmd_list;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == REDIR_HEREDOC)
			{
				if (process_heredoc_forked(r) == -1)
					return ;
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
}
