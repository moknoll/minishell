/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:59:19 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/04/24 12:53:05 by moritzknoll      ###   ########.fr       */
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
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(tokens, "<<", HEREDOC, NO_QUOTE);
		*i += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(tokens, ">>", REDIRECT_APPEND, NO_QUOTE);
		*i += 2;
	}
	else if (input[*i] == '|')
	{
		add_token(tokens, "|", PIPE, NO_QUOTE);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(tokens, "<", REDIRECT_IN, NO_QUOTE);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(tokens, ">", REDIRECT_OUT, NO_QUOTE);
		(*i)++;
	}
}

static void parse_quoted_word(char *input, int *i, t_token **tokens)
{
	int start;
	char quote_char;
	e_quote_type quote_type;
	char *word;

	quote_char = input[*i];
	if (quote_char == '\'')
		quote_type = SINGLE_QUOTE;
	else
		quote_type = DOUBLE_QUOTE;
	(*i)++; 	//skip opening quote
	start = *i;
	while(input[*i] && input[*i] != quote_char)
		(*i)++;
	word = ft_strndup(&input[start], *i - start);
	add_token(tokens, word, WORD, quote_type);
	free(word);
	if (input[*i] == quote_char)
		(*i)++; 	//skip closing quote

}

static void parse_unquoted_word(char *input, int *i, t_token **tokens)
{
	int start = *i;
	char *word;

	// Read until space, operator, or quote
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]) &&input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	if (*i > start)
	{
		word = ft_strndup(&input[start], *i - start);
		add_token(tokens, word, WORD, NO_QUOTE);
		free(word);
	}
}


static void handle_word(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '\'' || input[*i] == '"')
		parse_quoted_word(input, i, tokens);
	else
		parse_unquoted_word(input, i, tokens);
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
