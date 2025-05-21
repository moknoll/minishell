/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:24:53 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/21 12:41:02 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int create_pipe(t_command *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("create pipe");
			return -1;
		}
		return 1; // pipe was created
	}
	return 0; // No pipe needed
}

void child_process(t_command *cmd, int prev_fd, int pipefd[2])
{}

void execute_child_process(t_command *cmd, char **env)
{}

int parent_cleanup_and_wait(pid_t pid, int *prev_fd, int pipefd[2], t_command *cmd)
{}

void ft_pipe(t_command *cmd_list, char *env[])
{
	int pipefd[2];
	int prev_fd;
	pid_t pid;

	prev_fd = -1;
	while(cmd_list)
	{
		if (create_pipe(cmd_list, pipefd) == -1)
			return;
		pid = fork();
		if (pid == -1)
		{
			perror("fork"); //debugging
			return ;
		}
		if (pid == 0)
		{
			child_process(cmd_list, prev_fd, pipefd);
			execute_child_process(cmd_list, env);
		}
		else
		{
			parent_cleanup_and_wait(pid, &prev_fd, pipefd, cmd_list);
			cmd_list = cmd_list->next;
		}
	}
}
