/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:35 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/26 00:50:01 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

char	*get_variable_value_from_env(t_env *my_env, char *var_name)
{
	if (!var_name)
	{
		return (NULL);
	}
	while (my_env)
	{
		if (ft_strcmp(my_env->key, var_name) == 0 && my_env->exported)
			return (ft_strdup(my_env->value));
		my_env = my_env->next;
	}
	return (ft_strdup(""));
}

char	*get_variable_value(char *input, int *i, t_env *my_env)
{
	int		start;
	char	*var_name;
	char	*var_value;

	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_strndup(&input[start], *i - start);
	var_value = get_variable_value_from_env(my_env, var_name);
	free(var_name);
	if (!var_value)
		var_value = "";
	return (ft_strdup(var_value));
}

static void	handle_dollar(char *input, int *i, char **output, t_expand_ctx *ctx)
{
	char	*exit_status_str;
	char	*var_value;

	(*i)++;
	if (input[*i] == '?')
	{
		exit_status_str = ft_itoa(*(ctx->g_exit_status));
		append_str(exit_status_str, output);
		free(exit_status_str);
		(*i)++;
	}
	else if (ft_isalpha(input[*i]) || input[*i] == '_')
	{
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
		var_value = get_variable_value(input, i, ctx->my_env);
		append_str(var_value, output);
		free(var_value);
	}
	else
		append_char('$', output);
}

char	*expand_token(t_token *token, t_expand_ctx *ctx)
{
	const char	*input;
	char		*output;
	int			i;

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
			handle_dollar((char *)input, &i, &output, ctx);
		else
		{
			append_char(input[i], &output);
			i++;
		}
	}
	return (output);
}

void	expand_tokens(t_token *tokens, int *g_exit_status, t_env *env)
{
	t_token			*current;
	char			*expanded_value;
	t_expand_ctx	ctx;

	ctx.g_exit_status = g_exit_status;
	ctx.my_env = env;
	current = tokens;
	while (current)
	{
		if (current->type == WORD)
		{
			expanded_value = expand_token(current, &ctx);
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
