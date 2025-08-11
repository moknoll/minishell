/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 13:58:33 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	toggle_single_quote_state(char *input, int *i, int *in_single_quotes,
		int in_double_quotes)
{
	if (input[*i] == '\'' && !in_double_quotes)
	{
		*in_single_quotes = !(*in_single_quotes);
		(*i)++;
	}
}

void	toggle_double_quote_state(char *input, int *i, int in_single_quotes,
		int *in_double_quotes)
{
	if (input[*i] == '"' && !in_single_quotes)
	{
		*in_double_quotes = !(*in_double_quotes);
		(*i)++;
	}
}

void	expand_or_append_character(t_expand_state *state)
{
	if (state->input[state->i] == '$' && !state->in_single_quotes)
		expand_dollar_expression((char *)state->input, &state->i,
			&state->output, state->env);
	else
	{
		concatenate_character(state->input[state->i], &state->output);
		state->i++;
	}
}

t_expand_state	create_expansion_state(const char *input, t_env *env)
{
	t_expand_state	state;

	state.input = input;
	state.i = 0;
	state.output = NULL;
	state.env = env;
	state.in_single_quotes = 0;
	state.in_double_quotes = 0;
	return (state);
}

char	*expand_variables_and_remove_quotes(char *input, t_env *my_env)
{
	t_expand_state	state;

	if (!input)
		return (NULL);
	state = create_expansion_state(input, my_env);
	while (state.input[state.i])
	{
		if (state.input[state.i] == '\'' && !state.in_double_quotes)
		{
			state.in_single_quotes = !state.in_single_quotes;
			state.i++;
		}
		else if (state.input[state.i] == '"' && !state.in_single_quotes)
		{
			state.in_double_quotes = !state.in_double_quotes;
			state.i++;
		}
		else
			expand_or_append_character(&state);
	}
	if (!state.output)
		return (ft_strdup(""));
	return (state.output);
}
