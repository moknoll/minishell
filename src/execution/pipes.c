/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 13:57:39 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_pipes(char **args)
{
	int	i;
	int	pipe_count;

	i = 0;
	pipe_count = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			pipe_count++;
		i++;
	}
	return (pipe_count);
}

char	***split_all_pipe_commands(char **args, int pipe_count)
{
	char	***commands;
	int		cmd_index;
	int		start;
	int		i;
	t_range	r;

	commands = malloc(sizeof(char **) * (pipe_count + 2));
	if (!commands)
		return (NULL);
	cmd_index = 0;
	start = 0;
	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
		{
			r.start = start;
			r.end = i;
			process_pipe_segment(commands, args, &cmd_index, r);
			start = i + 1;
		}
		i++;
	}
	commands[cmd_index] = create_command_from_args(args, start, i);
	return (commands[cmd_index + 1] = NULL, commands);
}

void	free_commands(char ***commands)
{
	int	i;

	if (!commands)
		return ;
	i = 0;
	while (commands[i])
	{
		ft_free_tab(commands[i]);
		i++;
	}
	free(commands);
}

