/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:04:11 by mknoll            #+#    #+#             */
/*   Updated: 2025/06/27 15:52:34 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**env_to_sorted_array(t_env *env)
{
	int		count;
	char	**arr;
	int		i;
	char	*tmp;

	count = env_size(env);
	i = 0;
	arr = malloc(sizeof(char *) * (count + 1));
	while (env)
	{
		tmp = ft_strjoin(env->key, "");
		if (env->value)
			tmp = ft_strjoin_free(tmp, ft_strjoin("=", env->value));
		arr[i++] = tmp;
		env = env->next;
	}
	arr[i] = NULL;
	ft_sort_str_array(arr);
	return (arr);
}

int	ft_export(t_env *env)
{
	char	**arr;
	int		i;

	i = 0;
	arr = env_to_sorted_array(env);
	if (!arr)
	{
		printf("minishell: export: allocation error\n");
		return (1);
	}
	while (arr[i])
	{
		char	*equal = ft_strchr(arr[i], '=');

		if (equal)
		{
			// Trenne key und value an '='
			*equal = '\0';
			printf("declare -x %s=\"%s\"\n", arr[i], equal + 1);
			*equal = '='; // Rückgängig machen (optional, für Debugging/Freigabe)
		}
		else
			printf("declare -x %s\n", arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}


t_env *get_env(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}


int set_env_export_only(t_env **my_env, const char *key)
{
	t_env *var = get_env(*my_env, key);
	if (var)
		var->exported = 1;
	else
	{
		t_env *new = malloc(sizeof(t_env));
		new->key = ft_strdup(key);
		new->value = NULL;
		new->exported = 1;
		new->next = *my_env;
		*my_env = new;
	}
	return (0);
}

int	handle_export(char **argv, t_env **my_env)
{
	char	*eq;

	if (!argv[1])
		return (ft_export(*my_env));
	eq = ft_strchr(argv[1], '=');
	if (eq)
	{
		*eq = '\0';
		if (set_env(my_env, argv[1], eq + 1) != 0)
            return 1; 
		return (0);
	}
	return (set_env_export_only(my_env, argv[1]));
}
