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

static int	handle_heredoc_type(t_data *data, int i)
{
	if (!handle_heredoc_redirect(data, i))
		return (0);
	return (1);
}

static int	handle_append_type(t_data *data, int i)
{
	if (!handle_output_redirect(data, i, 1))
		return (0);
	return (1);
}

static int	handle_output_type(t_data *data, int i)
{
	if (!handle_output_redirect(data, i, 0))
		return (0);
	return (1);
}

static int	handle_input_type(t_data *data, int i)
{
	if (!handle_input_redirect(data, i))
		return (0);
	return (1);
}

int	process_single_redirect(t_data *data, int i)
{
	if (ft_strcmp(data->args[i], "<<") == 0)
		return (handle_heredoc_type(data, i));
	else if (ft_strcmp(data->args[i], ">>") == 0)
		return (handle_append_type(data, i));
	else if (ft_strcmp(data->args[i], ">") == 0)
		return (handle_output_type(data, i));
	else if (ft_strcmp(data->args[i], "<") == 0)
		return (handle_input_type(data, i));
	return (-1);
}
