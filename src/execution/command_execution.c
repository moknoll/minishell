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

static int prepare_redirections(char **cmd)
{
	int saved_stdout = dup(STDOUT_FILENO);
	int saved_stdin = dup(STDIN_FILENO);

	if (!process_redirections(cmd))
	{
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		g_exit_status = 1;
		return 0;
	}
	if (!cmd[0])
	{
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		return 0;
	}
	return 1;
}

static void execute_command(char **cmd, t_env *env, t_data *data)
{
	int exit_code = 0;
	if (is_builtin(cmd[0]))
	{
		exit_code = handle_builtin(cmd, &env, data);
		g_exit_status = exit_code;
	}
	else
		launch_extern_command_simple(cmd, env);
}

static void execute_single_command(char **cmd, t_env *env, t_data *data)
{
	int saved_stdout = dup(STDOUT_FILENO);
	int saved_stdin = dup(STDIN_FILENO);

	if (!prepare_redirections(cmd))
		return;
	execute_command(cmd, env, data);
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}

void execute(t_data *data)
{
	int pipe_count;
	char ***commands;

	if (!data->args || !data->args[0])
		return;
	pipe_count = count_pipes(data->args);
	commands = split_all_pipe_commands(data->args, pipe_count);
	if (!commands)
		return;
	if (pipe_count > 0)
	{
		execute_pipe_chain(commands, pipe_count + 1, *data->env, data);
	}
	else
	{
		execute_single_command(commands[0], *data->env, data);
	}
	free_commands(commands);
}
