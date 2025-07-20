/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	commands = malloc(sizeof(char **) * (pipe_count + 2));
	if (!commands)
		return (NULL);
	cmd_index = 0;
	start = 0;
	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			process_pipe_segment(commands, args, &cmd_index, &start, i);
		i++;
	}
	commands[cmd_index] = create_command_from_args(args, start, i);
	commands[cmd_index + 1] = NULL;
	return (commands);
}

void	free_commands(char ***commands)
{
	int	i;

	if (!commands)
		return;
	i = 0;
	while (commands[i])
	{
		ft_free_tab(commands[i]);
		i++;
	}
	free(commands);
}

int	process_cmd_redirections(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_strcmp(cmd[i], ">") == 0 && cmd[i + 1])
			return (handle_output_redirection(cmd, i));
		else if (ft_strcmp(cmd[i], ">>") == 0 && cmd[i + 1])
			return (handle_append_redirection(cmd, i));
		else if (ft_strcmp(cmd[i], "<") == 0 && cmd[i + 1])
			return (handle_input_redirection(cmd, i));
		else if (ft_strcmp(cmd[i], "<<") == 0 && cmd[i + 1])
		{
			// Pour l'instant, ignore les heredocs dans les pipes
			// car ils doivent être traités différemment
			i += 2;
			continue;
		}
		i++;
	}
	return (1);
}
