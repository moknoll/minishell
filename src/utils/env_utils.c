/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*env_array_to_list(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	char	*equal_pos;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp && envp[i])
	{
		equal_pos = ft_strchr(envp[i], '=');
		if (equal_pos)
		{
			new_node = malloc(sizeof(t_env));
			if (!new_node)
				break;
			new_node->key = ft_strndup(envp[i], equal_pos - envp[i]);
			new_node->value = ft_strdup(equal_pos + 1);
			new_node->exported = 1;
			new_node->next = env_list;
			env_list = new_node;
		}
		i++;
	}
	return (env_list);
}

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

int	set_env_value(t_env **env_list, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	if (!env_list || !key)
		return (FAILURE);
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (update_existing_env(current, value));
		current = current->next;
	}
	new_node = create_env_node(key, value);
	if (!new_node)
		return (FAILURE);
	new_node->next = *env_list;
	*env_list = new_node;
	return (SUCCESS);
}

int	unset_env_value(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env_list || !*env_list || !key)
		return (FAILURE);
	current = find_env_node(*env_list, key, &prev);
	if (!current)
		return (FAILURE);
	remove_env_node(env_list, current, prev);
	return (SUCCESS);
}

char	**env_to_array(t_env *env_list)
{
	char	**env_array;
	int		count;

	count = count_exported_vars(env_list);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	fill_env_array(env_array, env_list, count);
	return (env_array);
}
