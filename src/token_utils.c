/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:58:18 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/27 18:22:48 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *new_token(char *value, e_token_type type, e_quote_type quote_type, int has_space_before)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if(!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->quote_type = quote_type;
	token->has_space_before = has_space_before;
	token->next = NULL;
	return(token);
}

void add_token(t_token **head, char *value, e_token_type type, e_quote_type quote_type, int has_space_before)
{
	t_token *new;
	t_token *tmp;

	new = new_token(value, type, quote_type, has_space_before);
	if(!*head)
		*head = new;
	else
	{
		tmp = *head;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void free_tokens(t_token *head)
{
	t_token *tmp;

	while (head)
	{
		tmp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
	}
}

char **token_to_argv(t_token *token)
{
	int count;
	t_token *tmp;
	char **argv;
	int i;

	i = 0;
	tmp = token;
	count = 0;
	while(tmp)
	{
		count++;
		tmp = tmp->next;
	}
	argv = malloc(sizeof(char *) * count + 1);
	if(!argv)
		return (NULL);
	tmp = token;
	while(i < count)
	{
		argv[i] = ft_strdup(tmp->value);
		i++;
		tmp = tmp->next;
	}
	argv[count] = NULL;
	return (argv);
}

void print_tokens(t_token *tokens)
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
