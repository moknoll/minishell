/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:58:18 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/04/24 11:48:06 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

char	*ft_strdup(char *s)
{
	int		i;
	int		len;
	char	*copy;

	i = 0;
	len = 0;
	while (s[len])
	{
		len++;
	}
	copy = (char *) malloc(len + 1);
	if (copy == NULL)
		return (NULL);
	while (s[i])
	{
		copy[i] = s[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	*ft_strndup(char *s, size_t n)
{
	size_t		i;
	char	*copy;

	i = 0;
	copy = (char *) malloc(n + 1);
	if (copy == NULL)
		return (NULL);
	while (i < n && s[i])
	{
		copy[i] = s[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

t_token *new_token(char *value, e_token_type type, e_quote_type quote_type)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if(!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->quote_type = quote_type;
	token->next = NULL;
	return(token);
}

void add_token(t_token **head, char *value, e_token_type type, e_quote_type quote_type)
{
	t_token *new;
	t_token *tmp;

	new = new_token(value, type, quote_type);
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
