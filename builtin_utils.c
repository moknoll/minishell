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


void	builtin(char **argv, t_env **my_env, char **env)
{
	int	status;

	status = 0;
	(void)env;
	if (!argv[0])
		return ;
	if (ft_strcmp(argv[0], "exit") == 0)
		ft_exit(argv);
	else
		status = handle_builtin(argv, my_env);
	g_exit_status = status;
}
