/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_syntax.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:07:25 by mknoll            #+#    #+#             */
/*   Updated: 2025/06/26 13:51:17 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax_error(t_token *prev, t_token *curr)
{
	if (prev != NULL && prev->type == PIPE && curr->type == PIPE)
	{
		printf("syntax error: `|'\n");
		return (0);
	}
	if (prev != NULL && (prev->type >= REDIRECT_IN && prev->type <= HEREDOC))
	{
		if (curr->type != WORD)
		{
			if (curr->value != NULL)
				printf("syntax error: `%s'\n", curr->value);
			else
				printf("syntax error\n");
			return (0);
		}
	}
	return (1);
}


int	check_final_token(t_token *last)
{
	if (last != NULL)
	{
		if (last->type == PIPE)
		{
			printf("syntax error: unexpected end of input after pipe\n");
			return (0);
		}
		else if (last->type >= REDIRECT_IN && last->type <= HEREDOC)
		{
			if (last->value != NULL)
				printf("syntax error: missing file after redirection `%s'\n",
					last->value);
			else
				printf("syntax error: missing file after redirection\n");
			return (0);
		}
	}
	return (1);
}

int	check_syntax(t_token *tokens)
{
	t_token	*prev;

	if (!tokens)
		return (1);
	if (tokens->type == PIPE)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	prev = NULL;
	while (tokens)
	{
		if (!check_syntax_error(prev, tokens))
			return (0);
		prev = tokens;
		tokens = tokens->next;
	}
	return (check_final_token(prev));
}
