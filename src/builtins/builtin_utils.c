/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritz <moritz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 07:28:05 by mknoll            #+#    #+#             */
/*   Updated: 2025/07/28 14:05:38 by moritz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_multiple_n(char *str)
{
	char	valid;
	int		i;

	if (!str)
		return (0);
	i = 0;
	valid = 'n';
	while (str[i] && ft_strcmp(str, "-n"))
	{
		if (str[i] == '-')
			i++;
		if (str[i] != valid)
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo_n(char **argv)
{
	int	i;

	i = 2;
	{
		while (argv[i])
		{
			printf("%s", argv[i]);
			if (argv[i + 1])
				printf(" ");
			i++;
		}
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	free_all_and_exit(int exit_code, t_env *env, t_data *data)
{
    rl_clear_history();
    free_env_list(env);
    ft_free_tab(data->args);
    exit(exit_code);
}

int	ft_exit_simple(char **argv, t_env *env, t_data *data)
{
    int	exit_code;

    printf("exit\n");
	if (argv[1] && argv[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	if (argv[1])
    {
        exit_code = ft_atoi(argv[1]);
        if (exit_code < 0 || exit_code > 255)
            exit_code = exit_code % 256;
    }
    else
        exit_code = g_exit_status;
    free_all_and_exit(exit_code, env, data);
    return (0);
}

t_env	*get_env(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}
