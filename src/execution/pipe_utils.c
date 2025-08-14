/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/14 13:19:09 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*prepare_pipe_command_path(char **cmd, t_env *env, t_data *data,
	t_pipe_commands *pipe_cmds)
{
	char	*path;

	if (ft_strchr(cmd[0], '/'))
		path = ft_strdup(cmd[0]);
	else
		path = find_command_path(cmd[0], env);
	if (!path)
	{
		print_error(cmd[0], "command not found");
		free_commands(pipe_cmds->commands);
		free(path);
		free(pipe_cmds->pids);
		cleanup_pipes(pipe_cmds->pipes, pipe_cmds->command_count);
		free_all_and_exit(127, env, data);
	}
	return (path);
}

void	execute_external_pipe_command(char **cmd, t_env *env,
	t_data *data, t_pipe_commands *pipe_cmds)
{
	char	*path;
	char	**env_array;

	path = prepare_pipe_command_path(cmd, env, data, pipe_cmds);
	env_array = env_to_array(env);
	if (execve(path, cmd, env_array) == -1)
	{
		print_error(cmd[0], "No such file or directory");
		exit(127);
	}
}

int	**create_pipes(int cmd_count)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	i = 0;
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	setup_child_pipes(int **pipes, int cmd_count, int i)
{
	int	j;

	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < cmd_count - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	j = 0;
	while (j < cmd_count - 1)
	{
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

void	process_pipe_segment(char ***commands, char **args, int *cmd_index,
		t_range range)
{
	commands[*cmd_index] = create_command_from_args(args,
			range.start, range.end);
	(*cmd_index)++;
}
