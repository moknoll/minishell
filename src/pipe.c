/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:24:53 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/26 03:36:28 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipe(t_command *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("create pipe");
			return (-1);
		}
		return (1);
	}
	return (0);
}

void	execute_child_process(t_command *cmd, char **env)
{
	char	*path;

	path = get_path(cmd->argv[0], env);
	if (!path)
	{
		printf("Command not found: %s\n", cmd->argv[0]);
		exit(127);
	}
	execve(path, cmd->argv, env);
	perror("execve");
	exit(1);
}

static void	pipe_fork_and_exec(t_command *cmd_list, int prev_fd, int pipefd[2],
																	char *env[])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		child_process(cmd_list, prev_fd, pipefd, env);
		execute_child_process(cmd_list, env);
	}
	else
		parent_cleanup_and_wait(pid, &prev_fd, pipefd, cmd_list);
}

void	ft_pipe(t_command *cmd_list, char *env[])
{
	int	pipefd[2];
	int	prev_fd;

	prev_fd = -1;
	while (cmd_list)
	{
		if (create_pipe(cmd_list, pipefd) == -1)
			return ;
		pipe_fork_and_exec(cmd_list, prev_fd, pipefd, env);
		cmd_list = cmd_list->next;
	}
}
