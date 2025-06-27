/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:57:08 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/27 11:33:05 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_quotes(const char *input, int i)
{
	if (input[i] == '\'')
	{
		i++;
		while (input[i] && input[i] != '\'')
			i++;
		if (input[i] == '\'')
			i++;
	}
	else if (input[i] == '"')
	{
		i++;
		while (input[i] && input[i] != '"')
		{
			if (input[i] == '\\' && input[i + 1])
				i += 2;
			else
				i++;
		}
		if (input[i] == '"')
			i++;
	}
	return (i);
}

char	*strip_quotes(char *str)
{
	size_t	len;

	len = 0;
	if (!str)
		return (NULL);
	while (str[len])
		len++;
	if ((str[0] == '\'' && str[len - 1] == '\'')
		|| (str[0] == '"' && str[len - 1] == '"'))
		return (ft_strndup(str + 1, len - 2));
	return (ft_strdup(str));
}

void	strip_quotes_inplace(t_token *tokens)
{
	char	*stripped;

	while (tokens)
	{
		stripped = strip_quotes(tokens->value);
		free(tokens->value);
		tokens->value = stripped;
		tokens = tokens->next;
	}
}
