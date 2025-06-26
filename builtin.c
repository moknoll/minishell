/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:54:32 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/26 14:45:13 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_cd(char **argv, t_env **env)
{
	char	*path;
	char	cwd[PATH_MAX];

	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
	{
		path = get_variable_value_from_env(*env, "HOME");
		if (!path)
			return (perror("minishell: cd: HOME not set\n"), 1);
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		path = get_variable_value_from_env(*env, "OLDPWD");
		if (!path)
			return (perror("minishell: cd: OLDPWD not set\n"), 1);
	}
	else
		path = argv[1];
	if (chdir(path) == -1)
		return (printf("minishell: cd: %s: %s\n", path, strerror(errno)), 1);
	if (getcwd(cwd, sizeof(cwd)))
	{
		set_env(env, "OLDPWD", get_variable_value_from_env(*env, "PWD"));
		set_env(env, "PWD", cwd);
	}
	return (0);
}

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

static int	ft_pwd(char **argv)
{
	char	cwd[1024];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
	return (0);
}

static int	ft_echo(char **argv)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
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



void builtin(char **argv, t_env **my_env, char **env)
{
	int	status = 0;

	if (!argv[0])
		return ;
	if (ft_strcmp(argv[0], "exit") == 0)
		ft_exit(argv);
	else if (ft_strcmp(argv[0], "cd") == 0)
		status = ft_cd(argv, my_env);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		status = ft_pwd(argv);
	else if (ft_strcmp(argv[0], "echo") == 0)
	{
		if (argv[1] && check_multiple_n(argv[1]))
			status = ft_echo_n(argv);
		else
			status = ft_echo(argv);
	}
	else if (ft_strcmp(argv[0], "export") == 0)
		status = handle_export(argv, my_env);
	else if (ft_strcmp(argv[0], "unset") == 0)
		status = ft_unset(my_env, argv[1]);
	else if (ft_strcmp(argv[0], "env") == 0)
		status = ft_env(env);
	else
	{
		printf("minishell: %s: command not found\n", argv[0]);
		status = 127;
	}
	g_exit_status = status;
}
