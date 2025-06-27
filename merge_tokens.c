/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:11:03 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/27 10:21:51 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge_token(t_token **tokens)
{
	t_token	*current;
	t_token	*next;
	char	*merged;

	current = *tokens;
	while (current && current->next)
	{
		if (current->type == WORD && current->next
			&& current->next->type == WORD
			&& current->next->has_space_before == 0)
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
