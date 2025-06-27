/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:59:19 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/27 12:15:52 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_operator(char *input, int *i,
		t_token **tokens, int has_space)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(tokens, (t_token_data){"<<", HEREDOC, NO_QUOTE, has_space});
		*i += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(tokens, (t_token_data){">>", R_APPEND, NO_QUOTE, has_space});
		*i += 2;
	}
	else if (input[*i] == '|')
	{
		add_token(tokens, (t_token_data){"|", PIPE, NO_QUOTE, has_space});
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(tokens, (t_token_data){"<", R_IN, NO_QUOTE, has_space});
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(tokens, (t_token_data){">", R_OUT, NO_QUOTE, has_space});
		(*i)++;
	}
}

static int	parse_quoted_word(char *input, int *i,
		t_token **tokens, int has_space)
{
	int				start;
	char			quote_char;
	e_quote_type	quote_type;
	char			*word;
	char			*raw;

	quote_char = input[*i];
	if (quote_char == '\'')
		quote_type = SINGLE_QUOTE;
	else
		quote_type = DOUBLE_QUOTE;
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (!(input[*i] == quote_char))
		return (printf("Syntax error: unclosed %c-quote\n", quote_char), 0);
	raw = ft_strndup(&input[start], *i - start);
	word = ft_strtrim(raw, " \t\n\r\v\f");
	add_token(tokens, (t_token_data){word, WORD, quote_type, has_space});
	free(word);
	free(raw);
	(*i)++;
	return (1);
}

static void	parse_unquoted_word(char *input, int *i,
		t_token **tokens, int has_space)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& !is_operator(input[*i]) && input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	if (*i > start)
	{
		word = ft_strndup(&input[start], *i - start);
		add_token(tokens, (t_token_data){word, WORD, NO_QUOTE, has_space});
		free(word);
	}
}

int	handle_word(char *input, int *i, t_token **tokens, int has_space)
{
	if (input[*i] == '\'' || input[*i] == '"')
		return (parse_quoted_word(input, i, tokens, has_space));
	else
	{
		parse_unquoted_word(input, i, tokens, has_space);
		return (1);
	}
}

t_token	*tokenizer(char *input)
{
	t_token	*tokens;
	int		i;
	int		has_space;

	i = 0;
	has_space = 1;
	tokens = NULL;
	while (input[i])
	{
		if (!handle_token(input, &i, &tokens, &has_space))
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}
