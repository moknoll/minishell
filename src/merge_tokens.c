/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:11:03 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/06 12:36:38 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void merge_token(t_token **tokens)
{
	t_token *current = *tokens;
	t_token *next;
	char *merged;

	while (current && current->next)
	{
		if (current->type == WORD && current->next && current->next->type == WORD && current->next->has_space_before == 0)
		{
			next = current->next;
			merged = ft_strjoin(current->value, next->value);
			free(current->value);
			current->value = merged;
			current->next = next->next;
			free(next->value);
			free(next);
		}
		else
			current = current->next;
	}
}

