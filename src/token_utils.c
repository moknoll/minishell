/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:58:18 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/26 02:45:32 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*quote_type_str(e_quote_type qt)
{
	if (qt == NO_QUOTE)
		return ("NO_QUOTE");
	if (qt == SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	if (qt == DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	return ("UNKNOWN");
}

t_token	*new_token_struct(t_token_info *info)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(info->value);
	token->type = info->type;
	token->quote_type = info->quote_type;
	token->has_space_before = info->has_space_before;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token_info *info)
{
	t_token	*new;
	t_token	*tmp;

	new = new_token_struct(info);
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

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
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
	argv = malloc(sizeof(char *) * count + 1);
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

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: %-10s Type: %d Quote: %s\n",
			tokens->value,
			tokens->type,
			quote_type_str(tokens->quote_type));
		tokens = tokens->next;
	}
}
