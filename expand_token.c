/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:35 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/26 14:31:42 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

void	append_str(const char *str, char **output)
{
	char	*new;

	if (!str)
		return ;
	if (!*output)
	{
		*output = ft_strdup(str);
		return ;
	}
	new = ft_strjoin(*output, str);
	free(*output);
	*output = new;
}

void	append_char(char c, char **output)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	append_str(temp, output);
}

void	handle_dollar(char *input, int *i, char **output,
		int g_exit_status, t_env *my_env)
{
	char	*exit_status_str;
	char	*var_value;

	(*i)++;
	if (input[*i] == '?')
	{
		exit_status_str = ft_itoa(g_exit_status);
		append_str(exit_status_str, output);
		free(exit_status_str);
		(*i)++;
	}
	else if (ft_isalpha(input[*i]) || input[*i] == '_')
	{
		var_value = get_variable_value(input, i, my_env);
		append_str(var_value, output);
		free(var_value);
	}
	else
		append_char('$', output);
}

char	*expand_token(t_token *token, int g_exit_status, t_env *my_env)
{
	const char		*input;
	char			*output;
	int				i;

	input = token->value;
	output = NULL;
	i = 0;
	if (!input)
		return (NULL);
	if (token->quote_type == SINGLE_QUOTE)
		return (ft_strdup(token->value));
	output = calloc(1, 1);
	while (input[i])
	{
		if (input[i] == '$' && token->quote_type != SINGLE_QUOTE)
			handle_dollar((char *)input, &i, &output, g_exit_status, my_env);
		else
		{
			append_char(input[i], &output);
			i++;
		}
	}
	return (output);
}

void	expand_tokens(t_token *tokens, int g_exit_status, t_env *env)
{
	t_token	*current;
	char	*expanded_value;

	current = tokens;
	while (current)
	{
		if (current->type == WORD)
		{
			expanded_value = expand_token(current, g_exit_status, env);
			if (!expanded_value)
			{
				current = current->next;
				continue ;
			}
			free(current->value);
			current->value = expanded_value;
		}
		current = current->next;
	}
}
