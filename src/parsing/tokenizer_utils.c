/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unclosed_quotes(char *line)
{
	int		i;
	int		in_quotes;
	char	quote_char;

	i = 0;
	in_quotes = 0;
	quote_char = 0;
	while (line[i])
	{
		if ((line[i] == '"' || line[i] == '\'') && !in_quotes)
		{
			in_quotes = 1;
			quote_char = line[i];
		}
		else if (line[i] == quote_char && in_quotes)
		{
			in_quotes = 0;
			quote_char = 0;
		}
		i++;
	}
	return (in_quotes);
}

void	advance_in_token(char *line, int *i, int *in_quotes, char *quote_char)
{
	while (line[*i])
	{
		if ((line[*i] == '"' || line[*i] == '\'') && !*in_quotes)
		{
			*in_quotes = 1;
			*quote_char = line[*i];
		}
		else if (line[*i] == *quote_char && *in_quotes)
		{
			*in_quotes = 0;
			*quote_char = 0;
		}
		else if (!*in_quotes && (line[*i] == ' ' || line[*i] == '\t' || 
				line[*i] == '<' || line[*i] == '>' || line[*i] == '|'))
			break;
		(*i)++;
	}
}

void	process_token(char *line, int *i, int *count, int *in_quotes, 
		char *quote_char)
{
	if (!*in_quotes && line[*i] == '<' && line[*i + 1] == '<')
		*i += 2;
	else if (!*in_quotes && line[*i] == '>' && line[*i + 1] == '>')
		*i += 2;
	else if (!*in_quotes && (line[*i] == '<' || line[*i] == '>'
			|| line[*i] == '|'))
		(*i)++;
	else
		advance_in_token(line, i, in_quotes, quote_char);
	(*count)++;
}

int	count_tokens(char *line)
{
	int		i;
	int		count;
	int		in_quotes;
	char	quote_char;

	count = 0;
	i = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i])
		{
			in_quotes = 0;
			quote_char = 0;
			process_token(line, &i, &count, &in_quotes, &quote_char);
		}
	}
	return (count);
}

char	*extract_token(char *line, int *i)
{
	int		start;
	int		in_quotes;
	char	quote_char;

	start = *i;
	in_quotes = 0;
	quote_char = 0;
	if (!in_quotes && line[*i] == '<' && line[*i + 1] == '<')
		*i += 2;
	else if (!in_quotes && line[*i] == '>' && line[*i + 1] == '>')
		*i += 2;
	else if (!in_quotes && (line[*i] == '<' || line[*i] == '>' || line[*i] == '|'))
		(*i)++;
	else
		advance_in_token(line, i, &in_quotes, &quote_char);
	return (ft_substr(line, start, *i - start));
}
