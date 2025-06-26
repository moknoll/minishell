/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:05:19 by radubos           #+#    #+#             */
/*   Updated: 2025/06/26 02:15:25 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

static t_env	*create_env_node(const char *key, const char *value)
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
		current = current->next;
	}
	printf("unset done");
	return (0);
}
