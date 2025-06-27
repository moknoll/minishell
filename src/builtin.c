/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:54:32 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/26 23:12:03 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_exit(char **argv)
{
	if (argv[1])
		perror("exit: too many arguments\n");
	else
	{
		printf("exit\n");
		exit(0);
	}
}

static int	ft_export(t_env *env)
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

static int	handle_export(char **argv, t_env **my_env)
{
	char	*eq;

	if (!argv[1])
		return (ft_export(*my_env));
	eq = ft_strchr(argv[1], '=');
	if (eq)
	{
		*eq = '\0';
		return (set_env(my_env, argv[1], eq + 1));
	}
	return (set_env(my_env, argv[1], ""));
}

static int	handle_builtin(char **argv, t_env **my_env, char **env)
{
	if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(argv, my_env));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_pwd(argv));
	else if (ft_strcmp(argv[0], "echo") == 0)
		return (ft_echo(argv));
	else if (ft_strcmp(argv[0], "export") == 0)
		return (handle_export(argv, my_env));
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_unset(my_env, argv[1]));
	else if (ft_strcmp(argv[0], "env") == 0)
		return (ft_env(env));
	else
	{
		printf("minishell: %s: command not found\n", argv[0]);
		return (127);
	}
}

void	builtin(char **argv, t_env **my_env, char **env)
{
	int	status;

	status = 0;
	if (!argv[0])
	{
		printf("DEBUG: argv[0] is NULL\n");
		return ;
	}
	if (ft_strcmp(argv[0], "exit") == 0)
		ft_exit(argv);
	else
		status = handle_builtin(argv, my_env, env);
	g_exit_status = status;
}
