/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:14:28 by mknoll            #+#    #+#             */
/*   Updated: 2025/08/18 10:08:30 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

void	ft_sort_str_array(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	skip_whitespace(char *line, int *i)
{
	while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
		(*i)++;
}

void	free_all_and_exit(int exit_code, t_env *env, t_data *data)
{
	rl_clear_history();
	free_env_list(env);
	ft_free_tab(data->args);
	exit(exit_code);
}

void	cleanup_pipe_commands(t_pipe_commands *pipe_cmds)
{
	if (pipe_cmds->commands)
		free_commands(pipe_cmds->commands);
	if (pipe_cmds->pipes)
		cleanup_pipes(pipe_cmds->pipes, pipe_cmds->command_count);
	if (pipe_cmds->pids)
		free(pipe_cmds->pids);
}
