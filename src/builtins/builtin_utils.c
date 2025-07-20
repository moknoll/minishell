/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 07:28:05 by mknoll            #+#    #+#             */
/*   Updated: 2025/06/27 16:00:54 by mknoll           ###   ########.fr       */
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

int	ft_exit_simple(char **argv)
{
	int	exit_code;

	printf("exit\n");
	if (argv[1])
	{
		exit_code = ft_atoi(argv[1]);
		if (exit_code < 0 || exit_code > 255)
			exit_code = exit_code % 256;
	}
	else
		exit_code = g_exit_status;
	exit(exit_code);
	return (0);
}
