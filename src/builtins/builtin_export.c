/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env_custom(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->exported && current->value)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}

int	ft_unset(t_env **my_env, const char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!key || !my_env || !*my_env)
		return (1);
	current = *my_env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*my_env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

int	handle_unset(char **argv, t_env **my_env)
{
	int	i;
	int	result;

	if (!argv[1])
		return (0);
	result = 0;
	i = 1;
	while (argv[i])
	{
		if (ft_unset(my_env, argv[i]) != 0)
			result = 1;
		i++;
	}
	return (result);
}

int	process_export_arg(char *arg, t_env **my_env)
{
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		key = arg;
		value = equal_sign + 1;
		set_env_value(my_env, key, value);
		*equal_sign = '=';
	}
	else
	{
		key = arg;
		if (!get_env_value(*my_env, key))
			set_env_value(my_env, key, "");
	}
	return (0);
}

int	handle_export(char **argv, t_env **my_env)
{
	int	i;
	
	if (!argv[1])
	{
		ft_env_custom(*my_env);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		process_export_arg(argv[i], my_env);
		i++;
	}
	return (0);
}
