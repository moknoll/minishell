/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/11 11:49:58 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_variable_value_from_env(t_env *my_env, char *var_name)
{
	if (!var_name)
		return (NULL);
	while (my_env)
	{
		if (ft_strcmp(my_env->key, var_name) == 0)
			return (ft_strdup(my_env->value));
		my_env = my_env->next;
	}
	return (ft_strdup(""));
}

char	*extract_and_expand_variable(char *input, int *i, t_env *my_env)
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
		var_value = ft_strdup("");
	return (var_value);
}

void	concatenate_string(const char *str, char **output)
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

void	concatenate_character(char c, char **output)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	concatenate_string(temp, output);
}

void	expand_dollar_expression(char *input, int *i, char **output,
	t_env *my_env)
{
	char	*exit_status_str;
	char	*var_value;

	(*i)++;
	if (input[*i] == '?')
	{
		exit_status_str = ft_itoa(g_exit_status);
		concatenate_string(exit_status_str, output);
		free(exit_status_str);
		(*i)++;
	}
	else if (ft_isalpha(input[*i]) || input[*i] == '_')
	{
		var_value = extract_and_expand_variable(input, i, my_env);
		concatenate_string(var_value, output);
		free(var_value);
	}
	else
		concatenate_character('$', output);
}
