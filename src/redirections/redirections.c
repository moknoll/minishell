/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 14:06:18 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_heredoc_redirect(t_data *data, int i)
{
	char	*heredoc_file;
	int		saved_stdin;

	if (!validate_redirect_args(data, i))
		return (0);
	heredoc_file = handle_heredoc(data->args[i + 1]);
	if (!heredoc_file)
	{
		g_exit_status = 130;
		return (0);
	}
	if (setup_heredoc_fd(heredoc_file, &saved_stdin) == -1)
		return (0);
	remove_redirect_args(data, i);
	close(saved_stdin);
	free(heredoc_file);
	return (1);
}

int	handle_output_redirect(t_data *data, int i, int append)
{
	int	fd;

	if (!validate_redirect_args(data, i))
		return (0);
	fd = open_output_file(data->args[i + 1], append);
	if (fd == -1)
	{
		perror(data->args[i + 1]);
		g_exit_status = 1;
		free_env_list(*data->env);
		ft_free_tab(data->args);
		free(data->path);
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	remove_redirect_args(data, i);
	return (1);
}

int	handle_input_redirect(t_data *data, int i)
{
	int	fd;

	if (!validate_redirect_args(data, i))
		return (0);
	fd = open(data->args[i + 1], O_RDONLY);
	if (fd == -1)
	{
		perror(data->args[i + 1]);
		g_exit_status = 1;
		free_env_list(*data->env);
		return (0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	remove_redirect_args(data, i);
	return (1);
}

int	process_redirections(t_data *data)
{
	int	i;
	int	result;

	i = 0;
	while (data->args[i])
	{
		result = process_single_redirect(data, i);
		if (result == 0)
			return (0);
		if (result == 1)
			i = 0;
		else
			i++;
	}
	return (1);
}
