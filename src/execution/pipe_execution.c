/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/14 13:20:07 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_pipe_command(char **cmd, t_env *env, t_data *data,
	t_pipe_commands *pipe_cmds)
{
	int	exit_code;

	if (!process_cmd_redirections(cmd))
		exit(1);
	if (!cmd[0])
		exit(0);
	if (is_builtin(cmd[0]))
	{
		if (ft_strcmp(cmd[0], "exit") == 0)
			exit(0);
		exit_code = handle_builtin(cmd, &env, data);
		free_commands(pipe_cmds->commands);
		cleanup_pipes(pipe_cmds->pipes, pipe_cmds->command_count);
		free(pipe_cmds->pids);
		free_all_and_exit(exit_code, env, data);
	}
	else
		execute_external_pipe_command(cmd, env, data, pipe_cmds);
}

void	wait_for_children(pid_t *pids, int cmd_count)
{
	int	status;
	int	i;

	ignore_sigint();
	i = 0;
	while (i < cmd_count)
	{
		if (i == cmd_count - 1)
			waitpid(pids[i], &status, 0);
		else
			waitpid(pids[i], NULL, 0);
		i++;
	}
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	free(pids);
	init_signals_prompt();
}

void	init_pipe_commands(t_pipe_commands *pipe_cmds, int cmd_count,
	char ***commands)
{
	pipe_cmds->commands = commands;
	pipe_cmds->command_count = cmd_count;
	pipe_cmds->pipes = create_pipes(cmd_count);
	if (!pipe_cmds->pipes)
		return ;
}

void	execute_pipe_chain(char ***commands, int cmd_count,
		t_env *env, t_data *data)
{
	t_pipe_commands	pipe_cmds;
	int				i;

	init_pipe_commands(&pipe_cmds, cmd_count, commands);
	pipe_cmds.pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pipe_cmds.pids)
	{
		cleanup_pipes(pipe_cmds.pipes, cmd_count);
		return ;
	}
	i = 0;
	while (i < cmd_count)
	{
		pipe_cmds.pids[i] = fork();
		if (pipe_cmds.pids[i] == 0)
		{
			setup_child_pipes(pipe_cmds.pipes, cmd_count, i);
			reset_signals_to_default();
			execute_pipe_command(commands[i], env, data, &pipe_cmds);
			exit(0);
		}
		i++;
	}
	cleanup_pipes(pipe_cmds.pipes, cmd_count);
	wait_for_children(pipe_cmds.pids, cmd_count);
}

char	**create_command_from_args(char **args, int start, int end)
{
	char	**command;
	int		arg_index;

	command = malloc(sizeof(char *) * (end - start + 1));
	if (!command)
		return (NULL);
	arg_index = 0;
	while (start < end)
	{
		command[arg_index] = ft_strdup(args[start]);
		arg_index++;
		start++;
	}
	command[arg_index] = NULL;
	return (command);
}
