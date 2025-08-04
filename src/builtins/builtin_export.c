/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/04 13:38:32 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	char	*equal;

	i = 0;
	arr = env_to_sorted_array(env);
	if (!arr)
		return (printf("minishell: export: allocation error\n"), 1);
	while (arr[i])
	{
		equal = ft_strchr(arr[i], '=');
		if (equal)
		{
			*equal = '\0';
			printf("declare -x %s=\"%s\"\n", arr[i], equal + 1);
			*equal = '=';
		}
		else
			printf("declare -x %s\n", arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
	return (0);
}

int	set_env_export_only(t_env **my_env, const char *key)
{
	t_env	*var;
	t_env	*new;

	var = get_env(*my_env, key);
	if (var)
		var->exported = 1;
	else
	{
		new = malloc(sizeof(t_env));
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
	char	*key;
	char	*value;
	int		res;

	key = NULL;
	value = NULL;
	if (!argv[1])
		return (ft_export(*my_env));
	eq = ft_strchr(argv[1], '=');
	if (eq)
	{
		key = ft_substr(argv[1], 0, eq - argv[1]);
		value = ft_strdup(eq + 1);
		if (!key || !value)
			return (free(key), free(value), 1);
		res = set_env(my_env, key, value);
		free(key);
		free(value);
		return (res);
	}
	return (set_env_export_only(my_env, argv[1]));
}
