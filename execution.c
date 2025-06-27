/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:30:06 by radubos           #+#    #+#             */
/*   Updated: 2025/06/27 16:19:23 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_process(t_command *cmd, int prev_fd,
						int pipefd[2], char *env[])
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (apply_redirections(cmd->redirs) == -1)
	{
		perror("minishell: redirection");
		exit(1);
	}
	if (cmd->argv[0][0] == '/' || cmd->argv[0][0] == '.')
		execve(cmd->argv[0], cmd->argv, env);
	else
		execvp(cmd->argv[0], cmd->argv);
	perror("minishell: exec failed");
	exit(127);
}

int	handle_parent_process(int *prev_fd, int pipefd[2], int has_next)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (has_next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
	else
		*prev_fd = -1;
	return (0);
}

void	wait_for_children(pid_t *pids, int count)
{
	int	status;
	int	i;

	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else
				g_exit_status = 1;
		}
		i++;
	}
}

int	execute_external(t_command *cmd_list, char **env)
{
	int		pipefd[2];
	int		prev_fd;
	pid_t	pids[100];
	int		i;

	i = 0;
	prev_fd = -1;
	handle_heredocs(cmd_list);
	while (cmd_list)
	{
		if (cmd_list->next && pipe(pipefd) == -1)
			return (perror("minishell: pipe"), 0);
		pids[i] = fork();
		if (pids[i] < 0)
			return (perror("minishell: fork"), 0);
		else if (pids[i] == 0)
			handle_child_process(cmd_list, prev_fd, pipefd, env);
		handle_parent_process(&prev_fd, pipefd, cmd_list->next != NULL);
		cmd_list = cmd_list->next;
		i++;
	}
	wait_for_children(pids, i);
	if (prev_fd != -1)
		close(prev_fd);
	return (1);
}

int	execute_builtin_with_redirection(t_command *cmd,
		t_env **env, char **environ)
{
	int	saved_stdin;
	int	saved_stdout;
	int	redirected;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	redirected = 0;
	if (cmd->redirs)
	{
		if (apply_redirections(cmd->redirs) == -1)
			return (dup2(saved_stdin, STDIN_FILENO),
				dup2(saved_stdout, STDOUT_FILENO), close(saved_stdin),
				close(saved_stdout), 0);
		redirected = 1;
	}
	builtin(cmd->argv, env, environ);
	if (redirected)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
	}
	close(saved_stdin);
	close(saved_stdout);
	return (1);
}
