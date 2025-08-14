/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritz <moritz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/11 07:17:00 by moritz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_and_add_token(char *line, int *i, char ***tokens, int *count)
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

char	**prepare_token_array(char *line)
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

void	split_line_into_tokens(char *line, char **tokens)
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
			if (!parse_and_add_token(line, &i, &tokens, &count))
			{
				ft_free_tab(tokens);
				return ;
			}
		}
	}
	tokens[count] = NULL;
}

char	**tokenize(char *line)
{
	char	**tokens;

	if (!line)
		return (NULL);
	tokens = prepare_token_array(line);
	if (!tokens)
		return (NULL);
	split_line_into_tokens(line, tokens);
	return (tokens);
}
