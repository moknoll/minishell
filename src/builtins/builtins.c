/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritz <moritz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:54:32 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/08/12 14:02:59 by moritz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_cd_path(char **argv, t_env **env, char **allocated_path)
{
	char	*path;

	*allocated_path = NULL;
	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
	{
		path = get_variable_value_from_env(*env, "HOME");
		*allocated_path = path;
		if (!path || !*path)
			return (free(*allocated_path), perror("minishell: cd: HOME not set\n"), NULL);
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		path = get_variable_value_from_env(*env, "OLDPWD");
		*allocated_path = path;
		if (!path || !*path)
			return (free(*allocated_path), perror("minishell: cd: OLDPWD not set\n"),
				NULL);
	}
	else
		path = argv[1];
	return (path);
}

int	ft_cd(char **argv, t_env **env)
{
	char	*path;
	char	*allocated_path;
	char	cwd[4096];
	char	*old_pwd;
	int		result;

	path = get_cd_path(argv, env, &allocated_path);
	if (!path)
		return (1);
	result = 0;
	if (chdir(path) == -1)
	{
		printf("minishell: cd: %s: %s\n", path, strerror(errno));
		result = 1;
	}
	else if (getcwd(cwd, sizeof(cwd)))
	{
		old_pwd = get_variable_value_from_env(*env, "PWD");
		set_env(env, "OLDPWD", old_pwd);
		free(old_pwd);
		set_env(env, "PWD", cwd);
	}
	return (free(allocated_path), result);
}

int	ft_pwd(char **argv)
{
	char	cwd[1024];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
	return (0);
}

int	ft_echo(char **argv)
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

int	ft_exit_simple(char **argv, t_env *env, t_data *data)
{
	int	exit_code;

	printf("exit\n");
	if (argv[1] && !ft_isnumeric(argv[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n", argv[1]);
		g_exit_status = 2;
	}
	if (argv[1] && argv[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	if (argv[1] && ft_isnumeric(argv[1]))
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

int	handle_builtin(char **argv, t_env **my_env, t_data *data)
{
	if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(argv, my_env));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_pwd(argv));
	else if (ft_strcmp(argv[0], "echo") == 0)
	{
		if (check_multiple_n(argv[1]))
			return (ft_echo_n(argv));
		else
			return (ft_echo(argv));
	}
	else if (ft_strcmp(argv[0], "export") == 0)
		return (handle_export(argv, my_env));
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (handle_unset(argv, my_env));
	else if (ft_strcmp(argv[0], "env") == 0)
		return (ft_env_custom(*my_env));
	else if (ft_strcmp(argv[0], "exit") == 0)
		return (ft_exit_simple(argv, *my_env, data));
	else
	{
		print_error(argv[0], "command not found");
		return (127);
	}
}
