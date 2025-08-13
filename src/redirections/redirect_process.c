/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 14:05:16 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_heredoc_type(char **cmd, int i)
{
	if (!handle_heredoc_redirection(cmd, i))
		return (0);
	return (1);
}

static int	handle_append_type(char **cmd, int i)
{
	if (!handle_output_redirection(cmd, i, 1))
		return (0);
	return (1);
}

static int	handle_output_type(char **cmd, int i)
{
	if (!handle_output_redirection(cmd, i, 0))
		return (0);
	return (1);
}

static int	handle_input_type(char **cmd, int i)
{
	if (!handle_input_redirection(cmd, i))
		return (0);
	return (1);
}

int	process_cmd_redirections(char **cmd, int i)
{
	if (ft_strcmp(cmd[i], ">") == 0)
		return (handle_output_type(cmd, i));
	else if (ft_strcmp(cmd[i], ">>") == 0)
		return (handle_append_type(cmd, i));
	else if (ft_strcmp(cmd[i], "<") == 0)
		return (handle_input_type(cmd, i));
	else if (ft_strcmp(cmd[i], "<<") == 0)
		return (handle_heredoc_type(cmd, i));
	return (-1);
}