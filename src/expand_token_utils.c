/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 00:17:56 by radubos           #+#    #+#             */
/*   Updated: 2025/06/26 00:31:27 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_str(const char *str, char **output)
{
	char	*new;

	if (!str)
		return ;
	if (!*output)
	{
		*output = ft_strdup(str);
		return ;
	}
	new = ft_strjoin(*output, str);
	free(*output);
	*output = new;
}

void	append_char(char c, char **output)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	append_str(temp, output);
}
