/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_in_standard_paths(char *cmd)
{
	char	*standard_paths[] = {"/bin", "/usr/bin", "/usr/local/bin",
		"/opt/homebrew/bin", "/opt/local/bin", NULL};
	char	*full_path;
	char	*result;
	int		i;

	if (!cmd)
		return (NULL);
	i = 0;
	while (standard_paths[i])
	{
		full_path = build_full_path(standard_paths[i], cmd);
		if (!full_path)
		{
			i++;
			continue;
		}
		result = check_path_access(full_path);
		if (result)
			return (result);
		i++;
	}
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
	return (find_command_in_standard_paths(cmd));
}
