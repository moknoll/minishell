/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 13:49:37 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*build_full_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

char	*check_path_access(char *path)
{
	if (access(path, F_OK | X_OK) == 0)
		return (path);
	free(path);
	return (NULL);
}

char	*check_direct_path(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (NULL);
}

char	*search_in_path_env(char *cmd, char *path_env)
{
	char	**all_paths;
	char	*full_path;
	int		i;

	all_paths = ft_split(path_env, ':');
	if (!all_paths)
		return (NULL);
	i = 0;
	while (all_paths[i])
	{
		full_path = build_full_path(all_paths[i], cmd);
		if (full_path && access(full_path, F_OK | X_OK) == 0)
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

char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	*result;

	result = check_direct_path(cmd);
	if (result)
		return (result);
	path_env = get_env_value(env, "PATH");
	if (path_env)
	{
		result = search_in_path_env(cmd, path_env);
		if (result)
			return (result);
	}
	return (NULL);
}
