/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:58:18 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/04/22 14:15:48 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strdup(char *s)
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

t_token *new_token(char *value, e_token_type type)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if(!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return(token);
}

void add_token(t_token **head, char *value, e_token_type type)
{
	t_token *new;
	t_token *tmp;

	new = new_token(value, type);
	if(!head)
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

//ft_strndup
