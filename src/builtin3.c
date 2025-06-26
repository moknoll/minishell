/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 02:09:42 by radubos           #+#    #+#             */
/*   Updated: 2025/06/26 02:15:42 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_cd_path(char **argv, t_env **env)
{
	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
		return (get_variable_value_from_env(*env, "HOME"));
	else if (ft_strcmp(argv[1], "-") == 0)
		return (get_variable_value_from_env(*env, "OLDPWD"));
	else
		return (argv[1]);
}

static int	update_pwd_env(t_env **env)
{
	char	cwd[1024];
	char	*oldpwd;

	if (getcwd(cwd, sizeof(cwd)))
	{
		oldpwd = get_variable_value_from_env(*env, "PWD");
		if (oldpwd)
			set_env(env, "OLDPWD", oldpwd);
		else
			set_env(env, "OLDPWD", "");
		set_env(env, "PWD", cwd);
	}
	return (0);
}

int	ft_cd(char **argv, t_env **env)
{
	char	*path;

	path = get_cd_path(argv, env);
	if (!path)
	{
		if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
			perror("minishell: cd: HOME not set\n");
		else
			perror("minishell: cd: OLDPWD not set\n");
		return (1);
	}
	if (chdir(path) == -1)
	{
		printf("minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	update_pwd_env(env);
	return (0);
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
