/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:48:35 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/26 08:22:11 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		var_value = ft_strdup("");
	return (var_value);
}
