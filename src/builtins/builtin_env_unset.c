/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritz <moritz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 09:06:23 by moritz            #+#    #+#             */
/*   Updated: 2025/07/23 09:09:25 by moritz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env_custom(t_env *env)
{
	while (env)
	{
		if (env->value && env->exported)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
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