/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:34:54 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/05/27 22:36:13 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_readline(void)//we can use readline
{
	char *line;

	line = readline("minishell$ ");
	if(!line)
	{
		printf("exit\n");
		//herecod Ctrl+D
		exit(0);
	}
	if(*line)
		add_history(line);
	return (line);
}
