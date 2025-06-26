/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:30:06 by radubos           #+#    #+#             */
/*   Updated: 2025/06/26 00:09:55 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_redirections(t_redir *redirs)
{
	int	fd;
	int	std_fd;

	while (redirs)
	{
		if (open_redir_file(redirs, &fd, &std_fd) == -1)
			return (-1);
		if (fd < 0)
		{
			printf("minishell: %s: %s\n", redirs->file, strerror(errno));
			return (-1);
		}
		if (dup2(fd, std_fd) == -1)
		{
			printf("minishell: dup2 failed\n");
			close(fd);
			return (-1);
		}
		close(fd);
		redirs = redirs->next;
	}
	return (0);
}

static void	child_process(t_command *cmd, int prev_fd, int *pipefd, char **env)
{
	setup_child_signals();
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
	perror("minishell");
	exit(127);
}

static void	wait_for_last(pid_t last_pid, int *status)
{
	if (last_pid != -1)
	{
		waitpid(last_pid, status, 0);
		if (WIFEXITED(*status))
			g_exit_status = WEXITSTATUS(*status);
		else
			g_exit_status = 1;
	}
}

static pid_t	launch_child_and_manage_pipes(t_command *cmd, int *prev_fd,
														int *pipefd, char **env)
{
	pid_t	pid;

	if (cmd->next && pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		exit(1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		exit(1);
	}
	else if (pid == 0)
		child_process(cmd, *prev_fd, pipefd, env);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
	else
		*prev_fd = -1;
	return (pid);
}

void	execute_external(t_command *cmd_list, char **env)
{
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;
	int		last_pid;

	prev_fd = -1;
	last_pid = -1;
	handle_heredocs(cmd_list);
	while (cmd_list)
	{
		launch_child_and_manage_pipes(cmd_list, &prev_fd, pipefd, env);
		last_pid = pid;
		cmd_list = cmd_list->next;
	}
	wait_for_last(last_pid, &status);
	if (prev_fd != -1)
		close(prev_fd);
}
