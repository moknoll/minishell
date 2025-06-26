/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 01:05:36 by radubos           #+#    #+#             */
/*   Updated: 2025/06/26 01:06:34 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	parent_cleanup_and_wait(pid_t pid, int *prev_fd, int pipefd[2],
	t_command *cmd)
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
	return (0);
}
