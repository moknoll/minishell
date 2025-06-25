/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:24:53 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/25 20:03:32 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int create_pipe(t_command *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("create pipe");
			return -1;
		}
		return 1;
	}
	return 0;
}

void child_process(t_command *cmd, int prev_fd, int pipefd[2])
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
}

void	ft_free_tab(char **tab)
{
	int	i;

	if (!tab)
		perror("no tab");
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	*get_env_path(char *env[])
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			return (env[i] + 5);
		}
		i++;
	}
	return (NULL);
}

char	**split_path(char *envp[])
{
	char	*envp_path;
	char	**all_paths;

	envp_path = get_env_path(envp);
	if (!envp_path)
		return (NULL);
	all_paths = ft_split(envp_path, ':');
	return (all_paths);
}

char	*get_path(char *cmd, char *envp[])
{
	int		i;
	char	*full_path;
	char	*temp;
	char	**all_paths;

	all_paths = split_path(envp);
	i = 0;
	while (all_paths && all_paths[i])
	{
		temp = ft_strjoin(all_paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			ft_free_tab(all_paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_tab(all_paths);
	return (NULL);
}

void execute_child_process(t_command *cmd, char **env)
{
	char *path;

	path = get_path(cmd->argv[0], env);
	if(!path)
	{
		printf("Command not found: %s\n", cmd->argv[0]);
		exit(127);
	}
	execve(path, cmd->argv, env);
	perror("execve");
	exit(1);
}

int parent_cleanup_and_wait(pid_t pid, int *prev_fd, int pipefd[2], t_command *cmd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
	else
	{
		if (pipefd[0])
			close(pipefd[0]);
	}
	waitpid(pid, NULL, 0);
	return 0; 
}

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
			perror("fork");
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
