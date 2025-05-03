/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:54:32 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/04/23 11:20:36 by moritzknoll      ###   ########.fr       */
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
	else
		perror("command not found");
}
