/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:58:18 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/27 12:16:03 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value, e_token_type type, e_quote_type quote_type,
		int has_space_before)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->quote_type = quote_type;
	token->has_space_before = has_space_before;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token_data data)
{
	t_token	*new;
	t_token	*tmp;

	new = new_token(data.value, data.type,
			data.quote_type, data.has_space_before);
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

char	**token_to_argv(t_token *token)
{
	int		count;
	t_token	*tmp;
	char	**argv;
	int		i;

	i = 0;
	tmp = token;
	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	tmp = token;
	while (i < count)
	{
		argv[i] = ft_strdup(tmp->value);
		i++;
		tmp = tmp->next;
	}
	argv[count] = NULL;
	return (argv);
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	handle_token(char *input, int *i, t_token **tokens, int *has_space)
{
	if (ft_isspace(input[*i]))
	{
		(*has_space) = 1;
		(*i)++;
	}
	else if (is_operator(input[*i]))
	{
		handle_operator(input, i, tokens, *has_space);
		(*has_space) = 0;
	}
	else
	{
		if (!handle_word(input, i, tokens, *has_space))
			return (0);
		(*has_space) = 0;
	}
	return (1);
}
