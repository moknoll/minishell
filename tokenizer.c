/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:59:19 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/04/22 14:18:09 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int ft_isspace(char c)
{
	if (c == 32 || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

static int is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

static void handle_operator(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '|')
	{
		add_token(tokens, "|", PIPE);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(tokens, "<", REDIRECT_IN);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(tokens, ">", REDIRECT_OUT);
		(*i)++;
	}
	else if(input[*i] == '<' && input[*i+1] == '<')
	{
		add_token(tokens, "<<" ,HEREDOC);
		(*i) += 2;
	}
	else if(input[*i] == '>' && input[*i+1] == '>')
	{
		add_token(tokens, ">>", REDIRECT_APPEND);
		(*i) += 2;
	}
}

static void handle_word(char *input, int *i, t_token **tokens)
{
	int start;
	char *word;

	start = *i;
	while(input[*i] && !ft_isspace(input[*i] && !is_operator(input[*i])))
		(*i)++;
	word = ft_strndup(&input[start], *i - start);
	add_token(tokens, word, WORD);
	free(word);
}

t_token *tokenizer(char *input)
{
	t_token *tokens;
	int i;

	i = 0;
	tokens = NULL;
	while(input[i])
	{
		if(ft_isspace(input[i]))
			i++;
		else if (is_operator(input[i]))
			handle_operator(input, &i, &tokens);
		else
			handle_word(input, &i, &tokens);
	}
	return (tokens);
}
