/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 14:06:58 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->exported = 1;
	new->next = NULL;
	return (new);
}

int	set_env(t_env **env, const char *key, const char *value)
{
	t_env	*tmp;
	t_env	*new;

	tmp = *env;
	if (!key || !value)
		return (1);
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			tmp->exported = 1;
			return (0);
		}
		tmp = tmp->next;
	}
	new = create_env_node(key, value);
	if (!new)
		return (1);
	new->next = *env;
	*env = new;
	return (0);
}

t_env	*init_env(t_env **env, char **envp)
{
	char	*eq;
	char	*key;
	char	*value;

	while (*envp)
	{
		eq = ft_strchr(*envp, '=');
		if (eq)
		{
			key = ft_substr(*envp, 0, eq - *envp);
			value = ft_strdup(eq + 1);
			set_env(env, key, value);
			free(key);
			free(value);
		}
		envp++;
	}
	return (*env);
}

char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env	*find_env_node(t_env *env_list, char *key, t_env **prev)
{
	t_env	*current;

	current = env_list;
	*prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		*prev = current;
		current = current->next;
	}
	return (NULL);
}
