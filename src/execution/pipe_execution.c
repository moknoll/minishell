/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipe_command(char **cmd, t_env *env)
{
	if (!process_cmd_redirections(cmd))
		exit(1);
	if (!cmd[0])
		exit(0);
	if (is_builtin(cmd[0]))
	{
		int exit_code;
		if (ft_strcmp(cmd[0], "exit") == 0)
			exit(0);
		exit_code = handle_builtin(cmd, &env);
		exit(exit_code);
	}
	else
		execute_external_pipe_command(cmd, env);
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

void	execute_pipe_chain(char ***commands, int cmd_count, t_env *env)
{
	int		**pipes;
	pid_t	*pids;
	int		i;

	pipes = create_pipes(cmd_count);
	if (!pipes)
		return;
	pids = malloc(sizeof(pid_t) * cmd_count);
	i = 0;
	while (i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			setup_child_pipes(pipes, cmd_count, i);
			reset_signals_to_default();
			execute_pipe_command(commands[i], env);
			exit(0);
		}
		i++;
	}
	cleanup_pipes(pipes, cmd_count);
	wait_for_children(pids, cmd_count);
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

void	process_pipe_segment(char ***commands, char **args, int *cmd_index, 
		int *start, int i)
{
	commands[*cmd_index] = create_command_from_args(args, *start, i);
	(*cmd_index)++;
	*start = i + 1;
}
