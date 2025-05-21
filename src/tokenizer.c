/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:59:19 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/21 10:46:28 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_in_set(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
		{
			return (1);
		}
		set++;
	}
	return (0);
}

static size_t	ft_strlen(char const *str)
{
	int	len;

	len = 0;
	while (str[len])
	{
		len++;
	}
	return (len);
}

static char	*ft_strncpy(char *dest, char const *src, int n)
{
	int	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src [i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	int		len;
	char	*new_str;

	start = 0;
	end = ft_strlen(s1) - 1;
	while (s1[start] && is_in_set(s1[start], set))
		start++;
	while (end >= start && is_in_set(s1[end], set))
		end--;
	len = end - start + 1;
	new_str = (char *)malloc(len + 1);
	if (!new_str)
		return (NULL);
	ft_strncpy(new_str, &s1[start], len);
	new_str[len] = '\0';
	return (new_str);
}

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

static void handle_operator(char *input, int *i, t_token **tokens, int has_space)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(tokens, "<<", HEREDOC, NO_QUOTE, has_space);
		*i += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(tokens, ">>", REDIRECT_APPEND, NO_QUOTE, has_space);
		*i += 2;
	}
	else if (input[*i] == '|')
	{
		add_token(tokens, "|", PIPE, NO_QUOTE, has_space);
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(tokens, "<", REDIRECT_IN, NO_QUOTE, has_space);
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(tokens, ">", REDIRECT_OUT, NO_QUOTE, has_space);
		(*i)++;
	}
}

static int parse_quoted_word(char *input, int *i, t_token **tokens, int has_space)
{
	int start;
	char quote_char;
	e_quote_type quote_type;
	char *word;
	char *raw;

	quote_char = input[*i];
	if (quote_char == '\'')
		quote_type = SINGLE_QUOTE;
	else
		quote_type = DOUBLE_QUOTE;
	(*i)++; 	//skip opening quote
	start = *i;
	while(input[*i] && input[*i] != quote_char)
		(*i)++;
	if (!(input[*i] == quote_char))
	{
		printf("Syntax error: unclosed %c-quote\n", quote_char);
		return 0;
	}
	raw = ft_strndup(&input[start], *i - start);
	word = ft_strtrim(raw, " \t\n\r\v\f");
	add_token(tokens, word, WORD, quote_type, has_space);
	free(word);
	free(raw);
	(*i)++;
	return 1;
}

static void parse_unquoted_word(char *input, int *i, t_token **tokens, int has_space)
{
	int start = *i;
	char *word;

	// Read until space, operator, or quote
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]) && input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	if (*i > start)
	{
		word = ft_strndup(&input[start], *i - start);
		add_token(tokens, word, WORD, NO_QUOTE, has_space);
		free(word);
	}
}


static int handle_word(char *input, int *i, t_token **tokens, int has_space)
{
	if (input[*i] == '\'' || input[*i] == '"')
		return parse_quoted_word(input, i, tokens, has_space);
	else
	{
		parse_unquoted_word(input, i, tokens, has_space);
		return 1;
	}
}

t_token *tokenizer(char *input)
{
	t_token *tokens;
	int i;
	int has_space_before;

	has_space_before = 1;
	i = 0;
	tokens = NULL;
	while(input[i])
	{
		if(ft_isspace(input[i]))
		{
			i++;
			has_space_before = 1;
		}
		else if (is_operator(input[i]))
		{
			handle_operator(input, &i, &tokens, has_space_before);
			has_space_before = 0;
		}
		else
		{
			if(!handle_word(input, &i, &tokens, has_space_before))
			{
				free_tokens(tokens);
				return NULL;
			}
			has_space_before = 0;
		}
	}
	return (tokens);
}
