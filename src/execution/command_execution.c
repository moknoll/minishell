/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritz <moritz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/12 15:10:42 by moritz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_pipe_commands(t_data *data, int pipe_count)
{
	char	***commands;

	commands = split_all_pipe_commands(data->args, pipe_count);
	if (commands)
	{
		execute_pipe_chain(commands, pipe_count + 1, *data->env, data);
		free_commands(commands);
	}
}

void	execute_builtin_with_redirections(t_data *data)
{
	int	saved_stdout;
	int	saved_stdin;
	int	exit_code;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (!process_redirections(data))
	{
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		g_exit_status = 1;
		return ;
	}
	exit_code = handle_builtin(data->args, data->env, data);
	g_exit_status = exit_code;
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}

int has_redirection(t_data *data)
{
	int i = 0;
	while (data->args && data->args[i])
	{
		if (
			ft_strcmp(data->args[i], ">") == 0 ||
			ft_strcmp(data->args[i], ">>") == 0 ||
			ft_strcmp(data->args[i], "<") == 0 ||
			ft_strcmp(data->args[i], "<<") == 0 ||
			ft_strcmp(data->args[i], "|") == 0
		)
			return 1;
		i++;
	}
	return 0;
}

void	execute(t_data *data)
{
	int	pipe_count;

	if (!data->args || !data->args[0])
		return ;
	pipe_count = count_pipes(data->args);
	if (pipe_count > 0)
	{
		execute_pipe_commands(data, pipe_count);
		return ;
	}
	if (is_builtin(data->args[0]))
	{
		if (has_redirection(data))
			execute_builtin_with_redirections(data);
		else
			g_exit_status = handle_builtin(data->args, data->env, data);
	}
	else
		launch_extern_command_simple(data->args, *data->env);
}
