/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 14:05:30 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	remove_redirect_args(t_data *data, int i)
{
	char	*redirect_op;
	char	*filename;
	int		j;

	redirect_op = data->args[i];
	filename = data->args[i + 1];
	j = i;
	while (data->args[j + 2])
	{
		data->args[j] = data->args[j + 2];
		j++;
	}
	data->args[j] = NULL;
	data->args[j + 1] = NULL;
	free(redirect_op);
	free(filename);
}

int	setup_heredoc_fd(char *heredoc_file, int *saved_stdin)
{
	int	heredoc_fd;

	heredoc_fd = open(heredoc_file, O_RDONLY);
	if (heredoc_fd == -1)
	{
		perror("open heredoc");
		unlink(heredoc_file);
		free(heredoc_file);
		g_exit_status = 1;
		return (-1);
	}
	*saved_stdin = dup(STDIN_FILENO);
	dup2(heredoc_fd, STDIN_FILENO);
	close(heredoc_fd);
	return (0);
}

void	cleanup_heredoc_redirect(int saved_stdin, char *heredoc_file)
{
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	unlink(heredoc_file);
	free(heredoc_file);
}

int	open_output_file(char *filename, int append)
{
	int	fd;

	if (append)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		g_exit_status = 1;
		return (-1);
	}
	return (fd);
}

int	validate_redirect_args(t_data *data, int i)
{
	if (!data->args[i + 1])
	{
		print_error("minishell",
			"syntax error near unexpected token `newline'");
		g_exit_status = 2;
		return (0);
	}
	return (1);
}
