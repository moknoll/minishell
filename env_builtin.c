/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 07:29:10 by mknoll            #+#    #+#             */
/*   Updated: 2025/06/26 07:29:49 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			return (free(tmp->value), tmp->value = ft_strdup(value),
				tmp->exported = 1, 0);
		tmp = tmp->next;
	}
	new = malloc(sizeof(t_env));
	if (!new)
		return (1);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->exported = 1;
	new->next = *env;
	*env = new;
	return (0);
}

int	ft_env(char **env)
{
	if (!env || !*env)
	{
		printf("minishell: env: no environment\n");
		return (1);
	}
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
	return (0);
}


int	ft_unset(t_env **my_env, const char *key)
{
	t_env	*current;
	t_env	*previous;

	if (!key || !*key)
	{
		printf("minishell: unset: invalid identifier\n");
		return (1);
	}
	current = *my_env;
	previous = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (previous == NULL)
				*my_env = current->next;
			else
				previous->next = current->next;
			return (free(current->value), free(current->key), free(current), 0);
		}
		previous = current;
		current = current ->next;
	}
	printf("unset done");
	return (0);
}


int	ft_export(t_env *env)
{
	if (!env)
	{
		printf("minishell: export: no environment variables\n");
		return (1);
	}
	while (env)
	{
		if (env->exported)
		{
			printf("declare -x %s", env->key);
			if (env->value)
				printf("=\"%s\"", env->value);
			printf("\n");
		}
		env = env->next;
	}
	return (0);
}

int	handle_export(char **argv, t_env **env)
{
	int		status;
	char	*eq;
	int		i;

	i = 1;
	if (!argv[1])
		return (ft_export(*env));
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (eq)
		{
			*eq = '\0';
			status = set_env(env, argv[i], eq + 1);
			*eq = '=';
			if (status != 0)
				return (status);
		}
		else
		{
			status = set_env(env, argv[i], "");
			if (status != 0)
				return (status);
		}
		i++;
	}
	return (0);
}
