/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_single_quote(char *input, int *i, int *in_single_quotes, 
		int in_double_quotes)
{
	if (input[*i] == '\'' && !in_double_quotes)
	{
		*in_single_quotes = !(*in_single_quotes);
		(*i)++;
	}
}

void	handle_double_quote(char *input, int *i, int in_single_quotes, 
		int *in_double_quotes)
{
	if (input[*i] == '"' && !in_single_quotes)
	{
		*in_double_quotes = !(*in_double_quotes);
		(*i)++;
	}
}

void	process_character(char *input, int *i, char **output, t_env *my_env, 
		int in_single_quotes)
{
	if (input[*i] == '$' && !in_single_quotes)
		handle_dollar(input, i, output, my_env);
	else
	{
		append_char(input[*i], output);
		(*i)++;
	}
}

char	*expand_and_parse_token(char *input, t_env *my_env)
{
	char	*output;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;

	if (!input)
		return (NULL);
	output = NULL;
	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quotes)
			handle_single_quote(input, &i, &in_single_quotes, in_double_quotes);
		else if (input[i] == '"' && !in_single_quotes)
			handle_double_quote(input, &i, in_single_quotes, &in_double_quotes);
		else
			process_character(input, &i, &output, my_env, in_single_quotes);
	}
	if (!output)
		return (ft_strdup(""));
	return (output);
}

char	*heredoc_tmp(void)
{
	static int	i = 0;
	char		*num;
	char		*filename;

	num = ft_itoa(i++);
	filename = ft_strjoin("/tmp/heredoc_", num);
	if (!filename)
	{
		free(num);
		return (NULL);
	}
	free(num);
	return (filename);
}
