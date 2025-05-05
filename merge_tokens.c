/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:11:03 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/05 15:41:04 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void merge_token(t_token **tokens)
{
	char *merged;
	t_token *current;
	t_token *t_to_free;

	current = *tokens;
	while(current && current->next)
	{
		if (current->type == WORD && current->next->type == WORD)
		{
			merged = ft_strjoin(current->value, current->next->value);
			free(current->value);
			current->value = merged;
			t_to_free = current->next;
			current->next = t_to_free;
			free(t_to_free->value);
			free(t_to_free);
		}
		else
			current = current->next;
	}
}
