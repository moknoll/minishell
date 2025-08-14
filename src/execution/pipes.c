/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/14 13:19:54 by mknoll           ###   ########.fr       */
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

int	handle_heredoc_pipe(char **cmd, int i)
{
	char	*heredoc_file;
	int		saved_stdin;

	if (!cmd[i + 1])
		return (0);
	heredoc_file = handle_heredoc(cmd[i + 1]);
	if (!heredoc_file)
	{
		g_exit_status = 130;
		return (0);
	}
	if (setup_heredoc_fd(heredoc_file, &saved_stdin) == -1)
		return (0);
	remove_redirection_args(cmd, i);
	close(saved_stdin);
	free(heredoc_file);
	return (1);
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
			return (handle_heredoc_pipe(cmd, i));
		i++;
	}
	return (1);
}

void	cleanup_pipes(int **pipes, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}
