/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:59:50 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/26 18:49:56 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//not used anymore. i used another in execution.c

/* void execute_external(t_command *cmd, char **env)
{
	pid_t pid;
	int status;
	char *path;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		path = get_path(cmd->argv[0], env);
		if(!path)
		{
			printf("%s: command not found\n", cmd->argv[0]);
			exit(127);
		}
		execve(path, cmd->argv, env);
		perror("execve");
		exit(126);
	}
	else
	{
		waitpid(pid, NULL, 0);
		if(WIFEXITED(status))
			g_exit_status = WIFEXITED(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
	}
} */
