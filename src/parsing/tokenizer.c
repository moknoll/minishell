/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespace(char *line, int *i)
{
	while (line[*i] && (line[*i] == ' ' || line[*i] == '\t'))
		(*i)++;
}

int	process_single_token(char *line, int *i, char ***tokens, int *count)
{
	char	*token;

	token = extract_token(line, i);
	if (!token)
	{
		while (*count > 0)
			free((*tokens)[--(*count)]);
		free(*tokens);
		return (0);
	}
	(*tokens)[(*count)++] = token;
	return (1);
}

char	**allocate_tokens_array(char *line)
{
	char	**tokens;
	int		count;

	if (check_unclosed_quotes(line))
	{
		print_error("minishell", "syntax error: unclosed quotes");
		return (NULL);
	}
	count = count_tokens(line);
	tokens = malloc(sizeof(char *) * (count + 1));
	return (tokens);
}

void	tokenize_line(char *line, char **tokens)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (line[i])
	{
		skip_whitespace(line, &i);
		if (line[i])
		{
			if (!process_single_token(line, &i, &tokens, &count))
				return ;
		}
	}
	tokens[count] = NULL;
}

char	**tokenize(char *line)
{
	char	**tokens;

	if (!line)
		return (NULL);
	tokens = allocate_tokens_array(line);
	if (!tokens)
		return (NULL);
	tokenize_line(line, tokens);
	return (tokens);
}
