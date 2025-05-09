/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:54:32 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/09 12:01:50 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while(s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static void ft_cd(char **argv)
{
	char *path;

	if (argv[1])
		path = argv[1];
	else
		path = getenv("HOME");
	if (!path)
	{
		perror("cd: HOME not set\n");
		return;
	}
	if (chdir(path) == -1)
		perror("cd");
}

static void ft_exit(char **argv)
{
	if (argv[1])
		perror("exit: too many arguments\n");
	else
	{
		printf("exit\n");
		exit(0);
	}
}

static void ft_pwd(char **argv)
{
	char cwd[1024];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
}

static void ft_echo(char **argv)
{
	int count = 0;

	int i = 1;
	while(argv[count])
		count++;
	if (!argv[1])
		printf("\n");
	else
	{
		while(i < count)
		{
			printf("%s ", argv[i]);
			i++;
		}
		printf("\n");
	}
}

static void ft_echo_n(char **argv)
{
	int count = 0;
	int i = 2;

	while(argv[count])
		count++;
	if (!argv[2])
		return ;
	else
	{
		while(i < count)
		{
			printf("%s ", argv[i]);
			i++;
		}
	}
}

// static void ft_env(char **argv)
// {
// }

// static void ft_unset(char **argv)
// {

// }

// static void ft_export(char **argv)
// {

// }

void builtin(char **argv)
{
	if (!argv[0] || argv[0][0] == '\0')
		return;
	if (ft_strcmp(argv[0], "exit") == 0)
		ft_exit(argv);
	else if (ft_strcmp(argv[0], "cd") == 0)
		ft_cd(argv);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		ft_pwd(argv);
	else if (ft_strcmp(argv[0], "echo") == 0 && ft_strcmp(argv[1], "-n") == 0)
		ft_echo_n(argv);
	else if (ft_strcmp(argv[0], "echo") == 0)
		ft_echo(argv);
	// else if (ft_strcmp(argv[0], "unset") == 0)
	// 	ft_unset(argv);
	// else if (ft_strcmp(argv[0], "env") == 0)
	// 	ft_env(argv);
	// else if (ft_strcmp(argv[0], "export") == 0)
	// 	ft_export(argv);
	else
		perror("command not found");
}
