/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_advanced.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 14:08:56 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	update_existing_env(t_env *current, char *value)
{
	if (current->value)
		free(current->value);
	if (value)
		current->value = ft_strdup(value);
	else
		current->value = ft_strdup("");
	current->exported = 1;
	return (0);
}

int	count_exported_vars(t_env *env_list)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env_list;
	while (current)
	{
		if (current->exported && current->value)
			count++;
		current = current->next;
	}
	return (count);
}

static char	*create_env_string(t_env *current)
{
	char	*var_str;
	char	*result;

	var_str = ft_strjoin(current->key, "=");
	if (!var_str)
		return (NULL);
	result = ft_strjoin(var_str, current->value);
	free(var_str);
	return (result);
}

int	fill_env_array(char **env_array, t_env *env_list, int count)
{
	t_env	*current;
	int		i;

	i = 0;
	current = env_list;
	while (current && i < count)
	{
		if (current->exported && current->value)
		{
			env_array[i] = create_env_string(current);
			if (env_array[i])
				i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (i);
}

void	remove_env_node(t_env **env_list, t_env *current, t_env *prev)
{
	if (prev)
		prev->next = current->next;
	else
		*env_list = current->next;
	free(current->key);
	if (current->value)
		free(current->value);
	free(current);
}
